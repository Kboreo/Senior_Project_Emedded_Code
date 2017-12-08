/*
 * File:   Motor.c
 * Author: kboreo
 *
 * Created on November 15, 2017, 4:39 PM
 */
#include "motor.h"
#include "main.h"

void disableAllMotors(void)
{
    T1CON = 0x0; //disable timer1
    I11_L_SetHigh();
    I12_L_SetHigh();        //Disable all motors 
    I11_R_SetHigh();    
    I12_R_SetHigh();
}

void retractAllMotors()
{    
    //retractMotor(bothFront);
    //retractMotor(bothBack);    
}

void retractMotor(location loc)
{   
    if (loc == backRight)
    {
        forward = false;    //Set direction to reverse
        bothM = false;      //Set only one motor to drive
        leftM = false;      //Drive only Right motor
        
        volatile bool rightSwitch;  //bool for the limit switch on the right side
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        
        rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
        
        if (rightSwitch)    //Check if the right is enabled
            {                        
                enableMotor(bothBack);
                driveMotor(right);  //Drive the right motor in the selected direction.
                
                while(!rightLow)    //Drive motor until lower right limit switch has been hit
                {
                    
                }
                rightFlag = true;   //Set the flag for the right limit switch
                
                if (rightFlag)
                {
                    backOff(backRight);
                }
            }
    }
    
    if (loc == backLeft)
    {
        forward = false;    //Set direction to reverse
        bothM = false;      //Set only one motor to drive
        leftM = true;      //Drive only Left motor
        
        volatile bool leftSwitch;  //bool for the limit switch on the right side
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        
        leftSwitch = LIMIT_BL_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
        
        if (leftSwitch)    //Check if the right is enabled
            {                        
                enableMotor(bothBack);
                driveMotor(left);  //Drive the right motor in the selected direction.
                
                while(!leftLow) //Drive motor until lower left limit switch has been hit
                {
                    
                }
                leftFlag = true;   //Set the flag for the right limit switch
                
                if (leftFlag)
                {
                    backOff(backLeft);
                }
            }
    }
}

void backOff(location loc)
{
    forward = !forward;     //Flip direction that are currently going
    int target = 30;        //25 is approx 1mm
    wait = 0;       //reset wait var
    enableMotor(loc);   //Enable selected motor

    if(loc == backRight)
    {
        driveMotor(right);  //Drive the right motor in the selected direction.       	 
    }    
  
    if(loc == backLeft)
    {        
        driveMotor(left);       //Drive the left motor in the selected direction. 
    }    
   
    if(loc == bothBack)
    {
         driveMotor(both);       //Drive both of the motors selected.
    }
    
    while(wait <= target)   //Continue to drive motor for approx 1 mm.
    {

    }
    T1CON = 0x0; //disable timer1    
}

void extendMotor(location loc)
{
    if (loc == backLeft)
    {
        forward = true;    //Set direction to reverse
        bothM = false;      //Set only one motor to drive
        leftM = true;      //Drive only Left motor
        
        volatile bool leftSwitch;  //bool for the limit switch on the right side
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        
        leftSwitch = LIMIT_BL_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
        
        if (leftSwitch)    //Check if the right is enabled
            {                        
                enableMotor(bothBack);  //enable both back motors
                driveMotor(left);  //Drive the right motor in the selected direction.

                while (leftSwitch) //While the right switch is enabled, keep checking if the limit switch goes low.
                {
                    leftSwitch = LIMIT_BL_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
                }
                leftFlag = true;   //Set the flag for the right limit switch
                
                if (leftFlag)
                {
                    backOff(backLeft);
                }
            }
    }
    
    if (loc == backRight)
    {
        forward = true;    //Set direction to reverse
        bothM = false;      //Set only one motor to drive
        leftM = false;      //Drive only Right motor
        
        volatile bool rightSwitch;  //bool for the limit switch on the right side
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        
        rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
        
        if (rightSwitch)    //Check if the right is enabled
            {                        
                enableMotor(bothBack);
                driveMotor(right);  //Drive the right motor in the selected direction.

                while (rightSwitch) //While the right switch is enabled, keep checking if the limit switch goes low.
                {
                    rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
                }
                rightFlag = true;   //Set the flag for the right limit switch
                
                if (rightFlag)
                {
                    backOff(backRight);
                }
            }
    } 
    
    if(loc == bothBack)
    {
        forward = true;
        volatile bool leftSwitch;   //bool for the limit switch on the left side
        volatile bool rightSwitch;  //bool for the limit switch on the right side
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        
        rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
        leftSwitch = LIMIT_BL_GetValue();   //Set leftSwitch equal to the current value of the left limit switch.

        while ((rightSwitch + leftSwitch) > 0)
        {
            if (rightSwitch && leftSwitch)
            {
                enableMotor(bothBack);  //Enable both of the back motors
                driveMotor(both);       //Drive both of the motors selected.

                while (rightSwitch && leftSwitch) //Loop until the left and right limit switches have been hit. 
                {
                    rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
                    leftSwitch = LIMIT_BL_GetValue();   //Set leftSwitch equal to the current value of the left limit switch.				
                }
                leftFlag = true;    //Set the flag for the left limit switch
                rightFlag = true;   //Set the flag for the right limit switch                
                disableAllMotors(); //Disable all of the motors
            }
            
            if (leftFlag && rightFlag)  //Check if the flag for both limit switches has been set.
            {
                backOff(loc);   //Function to back leg off of limit switch
            }

            if (rightSwitch)    //Check if the right is enabled
            {
                enableMotor(backRight);
                driveMotor(right);  //Drive the right motor in the selected direction.

                while (rightSwitch) //While the right switch is enabled, keep checking if the limit switch goes low.
                {
                    rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
                }
                rightFlag = true;   //Set the flag for the right limit switch
                disableAllMotors(); //Disable all of the motors                        
            }
            
            if (rightFlag)
                {
                    backOff(loc);   //Function to back leg off of limit switch
                }

            if (leftSwitch)
            {
                enableMotor(backLeft);
                driveMotor(left);       //Drive the left motor in the selected direction.

                while (leftSwitch)
                {                
                    leftSwitch = LIMIT_BL_GetValue();   //Set leftSwitch equal to the current value of the left limit switch.				
                }
                leftFlag = true;    //Set the flag for the left limit switch
                disableAllMotors(); //Disable all of the motors
            }
            
            if (leftFlag)
            {
                backOff(loc);   //Function to back leg off of limit switch
            }
        }
        disableAllMotors(); //Disable all of the motors
        leftFlag = false;
        rightFlag = false;
    }
}

void enableMotor(location loc)
{
    switch (loc)
    {
            case backLeft:
                DEMUX_ENABLE_SetLow();  //Enable demux
                FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
                I11_L_SetLow();     //Enable left motors
                I12_L_SetLow();
                I11_R_SetHigh();     //Disable Right motors
                I12_R_SetHigh();
                break;
                
            case backRight:
                DEMUX_ENABLE_SetLow();  //Enable demux
                FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux
                I11_R_SetLow();     //Enable right motors
                I12_R_SetLow();
                I11_L_SetHigh();     //Disable Left motors                   
                I12_L_SetHigh();
                break;
                                
            case bothBack:
                DEMUX_ENABLE_SetLow();  //Enable demux
                FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
                I11_L_SetLow();
                I12_L_SetLow();
                I11_R_SetLow();     //Enable both left and right motors
                I12_R_SetLow();    
                break;
                
            default:
                break;
    }
}

void driveMotor(location loc)
{
    if(forward)
    {
        switch (loc)
        {
            case left:
                forward = true; //Motors are driving in the "forward" direction  
                leftM = true; //Only Left motor is driving
                bothM = false;   //Only one motor is driving
                
                _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
                TMR1 = 0; // Init the timer
                PR1 = 158080-1; // set the period register
                T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
                _T1IF = 0; //Clear Interrupt Flag
                _T1IE = 1; //Enable Clock Source
                PHASE1_L_SetHigh(); //Phase1 Left Motors
                PHASE2_L_SetHigh(); //Phase2 Left Motors
                break;
                
            case right:
                forward = true; //Motors are driving in the "forward" direction   
                leftM = false; //Only Right motor is driving
                bothM = false;   //Only one motor is driving
                
                _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
                TMR1 = 0; // Init the timer
                PR1 = 158080-1; // set the period register
                T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
                _T1IF = 0; //Clear Interrupt Flag
                _T1IE = 1; //Enable Clock Source  
                PHASE1_R_SetHigh(); //Phase1 Right Motors
                PHASE2_R_SetHigh(); //Phase2 Right Motors
                break;
                
            case both:
                forward = true; //Motors are driving in the "forward" direction  
                bothM = true;    //Both motors are moving
                _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
                TMR1 = 0; // Init the timer
                PR1 = 158080-1; // set the period register
                T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
                _T1IF = 0; //Clear Interrupt Flag
                _T1IE = 1; //Enable Clock Source
                PHASE1_L_SetHigh(); //Phase1 Left Motors
                PHASE2_L_SetHigh(); //Phase2 Left Motors    
                PHASE1_R_SetHigh(); //Phase1 Right Motors
                PHASE2_R_SetHigh(); //Phase2 Right Motors
                break;
                
            default:
                break;
        }
    }
    else
    {
        switch (loc)
        {
            case left:
                forward = false; //Motors are driving in the "reverse" direction   
                leftM = true; //Only left motor is driving
                bothM = false;   //Only one motor is driving
                _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
                TMR1 = 0; // Init the timer
                PR1 = 158080-1; // set the period register
                T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
                _T1IF = 0; //Clear Interrupt Flag
                _T1IE = 1; //Enable Clock Source    
                PHASE1_L_SetHigh(); //Phase1 Left Motors
                PHASE2_L_SetHigh(); //Phase2 Left Motors 
                break;
                
            case right:
                forward = false; //Motors are driving in the "reverse" direction    
                leftM = false; //Only Right motor is driving
                bothM = false;   //Only one motor is driving
                _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
                TMR1 = 0; // Init the timer
                PR1 = 158080-1; // set the period register
                T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
                _T1IF = 0; //Clear Interrupt Flag
                _T1IE = 1; //Enable Clock Source
                PHASE1_R_SetHigh(); //Phase1 Right Motors
                PHASE2_R_SetHigh(); //Phase2 Right Motors
                
                break;
                
            case both:
                forward = false; //Motors are driving in the "forward" direction   
                leftM = false; //Only Right motor is driving
                bothM = true;   //Only one motor is driving
                _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
                TMR1 = 0; // Init the timer
                PR1 = 158080-1; // set the period register
                T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
                _T1IF = 0; //Clear Interrupt Flag
                _T1IE = 1; //Enable Clock Source
                PHASE1_L_SetHigh(); //Phase1 Left Motors
                PHASE2_L_SetHigh(); //Phase2 Left Motors    
                PHASE1_R_SetHigh(); //Phase1 Right Motors
                PHASE2_R_SetHigh(); //Phase2 Right Motors
                break;
                
            default:
                break;
        }
    }
}

void initialMotorExtend(void)
{
    forward = true;    //Set direction to reverse
    bothM = true;      //Set only one motor to drive
    wait = 0;           //Set wait to 0
    
    enableMotor(bothBack);  //enable both back motors
    driveMotor(both);  //Drive both motors in the selected direction.
    
    while(wait<= 160)    //Drive both back motors for 160 steps (approx 0.25")
    {
        
    }
    T1CON = 0x0; //disable timer1    
}

void levelHorizontally(void)
{         
    while((tiltBackRight) || (tiltBackLeft))      //while board is not level, loop state machine
    {     
        if(tiltBackRight)   //If right side tilt switch is hit, meaning right side is low.
        {
            if(rightHigh && leftHigh)   //if both high limit switches have been hit, error occurred... exit
            {
                goto error; //jumps to error loop
            }

            if(!rightHigh) //if the high limit switch on the back right has not been hit
            {
                forward = true;
                enableMotor(bothBack);
                driveMotor(right);
                while(tiltBackRight && !rightHigh)  //While switch is still showing right side is low and right high limit switch has not been hit
                {                                   //Drive Back Right motor forward   

                }
                backOff(backRight);
                T1CON = 0x0; //disable timer1, which stops the motors from driving

                if(!tiltBackRight && !tiltBackLeft)
                {
                    goto level;
                }

                if(rightHigh)       //High limit switch was hit, need to check if board is level
                {
                    if(!tiltBackRight && !tiltBackLeft) //Board is level
                    {
                        backOff(bothBack);
                        goto level;
                    }

                    if(rightHigh && leftHigh)   //An error occurred
                    {
                        backOff(bothBack);
                        goto error;
                    }

                    if(!leftHigh) //left higher limit switch has not been hit, need to lower to try to level 
                    {
                        forward = false;
                        enableMotor(bothBack);
                        driveMotor(left);
                        while(tiltBackRight && !leftLow)  //While switch is still showing rightSidelow and lower left switch has not been hit
                        {                                   //Drive Back Right motor forward   

                        }
                        T1CON = 0x0; //disable timer1, which stops the motors from driving

                        if(!tiltBackRight && !tiltBackLeft)
                        {
                            backOff(backLeft);
                            goto level;
                        }

                        if(leftLow)
                        {
                            backOff(backLeft);
                            goto error;
                        }
                    }
                }
            }            
        }

        if(tiltBackLeft)   //If left side tilt switch is hit, meaning left side is low.)
        {
            if(rightHigh && leftHigh)   //if both high limit switches have been hit, error occurred... exit
            {
                goto error; //jumps to error loop
            }

            if(!leftHigh) //if the high limit switch on the back left has not been hit
            {
                forward = true;
                enableMotor(bothBack);
                driveMotor(left);
                while(tiltBackLeft && !leftHigh)  //While switch is still showing left side is low and left high limit switch has not been hit
                {                                     
                        //Drive Back left motor forward 
                }
                T1CON = 0x0; //disable timer1, which stops the motors from driving

                if(!tiltBackRight && !tiltBackLeft)
                {
                    goto level;
                }

                if(leftHigh)       //High limit switch was hit, need to check if board is level
                {
                    if(!tiltBackRight && !tiltBackLeft) //Board is level
                    {
                        goto level;
                    }

                    if(rightHigh && leftHigh)   //An error occurred
                    {
                        goto error;
                    }
                    if(!rightHigh) //right higher limit switch has not been hit, need to lower to try to level 
                    {
                        forward = false;
                        enableMotor(bothBack);
                        driveMotor(right);
                        while(tiltBackLeft && !rightLow)  //While switch is still showing rightSidelow and lower left switch has not been hit
                        {                                   //Drive Back Right motor forward   

                        }
                        T1CON = 0x0; //disable timer1, which stops the motors from driving

                        if(!tiltBackRight && !tiltBackLeft)
                        {
                            goto level;
                        }

                        if(rightLow)
                        {
                            backOff(backRight);
                            goto error;
                        }
                    }
                }
            }
        }
        error:
        {
            break;//something went wrong, break out of while loop
        }
        level:
        {
            break; //leveling has been completed
        }
    }        
}

void raise10Degrees(void)
{   
    while((tiltRightLow || tiltRightHigh) || (tiltLeftLow || tiltLeftHigh)) //While any switch is not level, loop
    {
        if((!tiltRightLow && !tiltRightHigh) || (!tiltLeftLow && !tiltLeftHigh))//Check if any limit switches have been hit, if not the board is level 
        {
            goto vertLevel;
        }

        if(tiltRightLow && (!rightHigh && !leftHigh))     //If neither side has hit the upper limit the board can be raised until level or high limit switch can be hit
        {
            forward = true;
            enableMotor(bothBack);  //Drive both motors in the forward direction
            driveMotor(both);
            while(((tiltRightLow || tiltRightHigh)) && (!rightHigh && !leftHigh)) //While not level and either high limit switch has not been hit, drive motors
            {

            }
            T1CON = 0x0; //disable timer1, which stops the motors from driving

            if((!tiltRightLow && !tiltRightHigh) || (!tiltLeftLow && !tiltLeftHigh))//Check if any limit switches have been hit, if not the board is level 
            {
                goto vertLevel;
            }        
        }

        if(tiltRightHigh && (!rightLow && !leftLow))//If neither side has hit the lower limits, the board can be lowered until level or lower limit switch is hit
        {
            forward = false;
            enableMotor(bothBack);  //Drive both motors in the reverse direction
            driveMotor(both);

            while(((tiltRightLow || tiltRightHigh)) && (!rightLow && !leftLow)) //While not level and either lower limit switch has not been hit, drive motors
            {

            }
            T1CON = 0x0; //disable timer1, which stops the motors from driving

            if((!tiltRightLow && !tiltRightHigh) || (!tiltLeftLow && !tiltLeftHigh))//Check if any limit switches have been hit, if not the board is level 
            {                
                goto vertLevel;
            }  
            else
            {               
                goto vertError; //either error occurred or board is more unlevel than system can fix
            }
        }   

        vertError:
        {
            backOff(bothBack);
            break;  //An error has occurred or 10 degrees can not be made
        }
        vertLevel:
        {
            backOff(bothBack);
            break;  //board is level break from loop
        }
    }
}
