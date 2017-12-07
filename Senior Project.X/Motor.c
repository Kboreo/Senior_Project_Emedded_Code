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
    retractMotor(bothBack);    
}

void retractMotor(location loc)
{
    if (loc == bothBack)    //Check if location is equal to "bothBack"
    {
        forward = false;    //Set direction to reverse
        volatile bool leftSwitch;   //bool for the limit switch on the left side
        volatile bool rightSwitch;  //bool for the limit switch on the right side
        volatile bool leftFlag = false;     //flag when left limit switch has been hit to prevent leg moving again after limit switch was hit
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit

        rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.
        leftSwitch = LIMIT_BL_GetValue();   //Set leftSwitch equal to the current value of the left limit switch.

        while (!rightFlag && !leftFlag)     //Loop while the flags for the limit switches have not been hit.
        {
            if (rightSwitch && leftSwitch)  //Check if both of the limit switches have been hit.
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
                
                if (leftFlag && rightFlag)  //Check if the flag for both limit switches has been set.
                {
                    backOff(loc);   //Function to back leg off of limit switch
                }
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
                if (rightFlag)
                {
                    backOff(backRight);
                }
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
                if(leftFlag)
                {
                    backOff(backLeft);
                }
            }		
        }
    disableAllMotors(); //Disable all of the motors
    }
    
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
}

void backOff(location loc)
{
    forward = !forward;
    int target = 25;        //25 is approx 1mm
    wait = 0;       //reset wait var
    enableMotor(loc);

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
    
    while(wait <= target)
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
                enableMotor(bothBack);
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
                bothM = true;    //
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
    
    while(wait<= 160)    //Drive both back motors for 80 steps (approx 0.25")
    {
        
    }
    T1CON = 0x0; //disable timer1    
}

void levelHorizontally(void)
{
    bool tiltBackRight; //When equal to one, right side is too low
    bool tiltBackLeft; //When equal to one, left side is too low
    
    
    //Get initial conditions from system
    tiltBackRight = TILT_BACK_RIGHT_GetValue();     //Sets tiltBackRight equal to TILT_BACK_RIGHT
    tiltBackLeft = TILT_BACK_LEFT_GetValue();       //Sets tiltBackLeft equal to TILT_BACK_LEFT
    
    if(tiltBackRight)   //If right side tilt switch is hit, meaning right side is low.
    {
        
        if(!rightHigh) //if the high limit switch has not been hit
        {
            
        }
    }
    

    
    while(TILT_BACK_RIGHT_GetValue())
    {
        
    }
    
}

