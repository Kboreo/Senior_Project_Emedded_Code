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
    retractMotor(bothFront);
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
    
    if (loc == bothFront)
    {
        forward = false;
        volatile bool leftSwitch;   //bool for the limit switch on the left side
        volatile bool rightSwitch;  //bool for the limit switch on the right side
        volatile bool leftFlag = false;     //flag when left limit switch has been hit to prevent leg moving again after limit switch was hit
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit

        rightSwitch = LIMIT_FR_GetValue();
        leftSwitch = LIMIT_FL_GetValue();

        while (!rightFlag && !leftFlag)
        {
            if (rightSwitch && leftSwitch)
            {			
                enableMotor(bothFront);
                driveMotor(both);       //Drive both of the motors selected.

                while (rightSwitch && leftSwitch) //Loop until the left and right limit switches have been hit. 
                {
                    rightSwitch = LIMIT_FR_GetValue();
                    leftSwitch = LIMIT_FL_GetValue();				
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
                enableMotor(frontRight);
                driveMotor(right);  //Drive the right motor in the selected direction.

                while (rightSwitch) //While the right switch is enabled, keep checking if the limit switch goes low.
                {
                    rightSwitch = LIMIT_FR_GetValue();
                }
                rightFlag = true;   //Set the flag for the right limit switch
                disableAllMotors(); //Disable all of the motors
                if (rightFlag)
                {
                    backOff(frontRight);
                }
            }

            if (leftSwitch)
            {
                enableMotor(frontLeft);
                driveMotor(left);       //Drive the left motor in the selected direction.

                while (leftSwitch)
                {                
                    leftSwitch = LIMIT_FL_GetValue();				
                }
                leftFlag = true;    //Set the flag for the left limit switch                
                disableAllMotors(); //Disable all of the motors
                if(leftFlag)
                {
                    backOff(frontLeft);
                }
            }		
        }
    disableAllMotors(); //Disable all of the motors        
    }    
}

void backOff(location loc)
{
    forward = !forward;
    int target = 25;
    wait = 0;
    enableMotor(loc);
    
    if(loc == frontRight)
    {
        driveMotor(right);  //Drive the right motor in the selected direction.       	 
    }
    
    if(loc == backRight)
    {
        driveMotor(right);  //Drive the right motor in the selected direction.       	 
    }
    
    if(loc == frontLeft)
    {        
        driveMotor(left);       //Drive the left motor in the selected direction. 
    }
    
    if(loc == backLeft)
    {        
        driveMotor(left);       //Drive the left motor in the selected direction. 
    }
    
    if(loc == bothFront)
    {
         driveMotor(both);       //Drive both of the motors selected.
    }
    
    if(loc == bothBack)
    {
         driveMotor(both);       //Drive both of the motors selected.
    }
    
    while(wait <= target)
    {

    }
    disableAllMotors(); //Disable all of the motors
}

void extendMotor(location loc)
{
    if(loc == frontRight)
    {        
        forward = true;
        volatile bool rightSwitch;  //bool for the limit switch on the right side    
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        rightSwitch = LIMIT_FR_GetValue();    
    
        if (rightSwitch)    //Check if the right is enabled
        {
            enableMotor(loc);
            driveMotor(right);  //Drive the right motor in the selected direction.

            while(rightSwitch) 
            {
                rightSwitch = LIMIT_FR_GetValue();            			
            }
            rightFlag = true;   //Set the flag for the right limit switch
            disableAllMotors(); //Disable all of the motors	
        }
        if(rightFlag)
        {
            backOff(loc);   //Function to back leg off of limit switch
        }
        disableAllMotors(); //Disable all of the motors
    }
    
    if(loc == frontLeft)
    {
        forward = true;
        volatile bool leftSwitch;  //bool for the limit switch on the right side    
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        leftSwitch = LIMIT_FL_GetValue();    
    
        if (leftSwitch)
        {
            enableMotor(loc);
            driveMotor(left);       //Drive the left motor in the selected direction.

            while(leftSwitch) 
            {
                leftSwitch = LIMIT_FL_GetValue();            			
            }
            leftFlag = true;    //Set the flag for the left limit switch
            disableAllMotors(); //Disable all of the motors	
        }
        if(leftFlag)
        {
            backOff(loc);   //Function to back leg off of limit switch
        }
        disableAllMotors(); //Disable all of the motors
    }
    
    if(loc == backLeft)
    {
        forward = true;
        volatile bool leftSwitch;  //bool for the limit switch on the right side    
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        leftSwitch = LIMIT_BL_GetValue();   //Set leftSwitch equal to the current value of the left limit switch.    
    
        if (leftSwitch)
        {
            enableMotor(loc);
            driveMotor(left);       //Drive the left motor in the selected direction.

            while(leftSwitch) 
            {
                leftSwitch = LIMIT_BL_GetValue();   //Set leftSwitch equal to the current value of the left limit switch.            			
            }
            leftFlag = true;    //Set the flag for the left limit switch
            disableAllMotors(); //Disable all of the motors	
        }
        if(leftFlag)
        {
            backOff(loc);   //Function to back leg off of limit switch
        }
        disableAllMotors(); //Disable all of the motors
    }
    
    if(loc == backRight)
    {
        forward = true;
        volatile bool rightSwitch;  //bool for the limit switch on the right side    
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.    
    
        if (rightSwitch)    //Check if the right is enabled
        {
            enableMotor(loc);
            driveMotor(right);  //Drive the right motor in the selected direction.

            while(rightSwitch) 
            {
                rightSwitch = LIMIT_BR_GetValue();  //Set rightSwitch equal to the current value of the right limit switch.            			
            }
            rightFlag = true;   //Set the flag for the right limit switch
            disableAllMotors(); //Disable all of the motors	
        }
        if(rightFlag)
        {
            backOff(loc);   //Function to back leg off of limit switch
        }
        disableAllMotors(); //Disable all of the motors
    }
    
    if(loc == bothFront)
    {
        forward = true;
        volatile bool leftSwitch;   //bool for the limit switch on the left side
        volatile bool rightSwitch;  //bool for the limit switch on the right side
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
        
        rightSwitch = LIMIT_FR_GetValue();
        leftSwitch = LIMIT_FL_GetValue();

        while ((rightSwitch + leftSwitch) > 0)
        {
            if (rightSwitch && leftSwitch)
            {
                enableMotor(bothBack);  //Enable both of the back motors
                driveMotor(both);       //Drive both of the motors selected.

                while (rightSwitch && leftSwitch) //Loop until the left and right limit switches have been hit. 
                {
                    rightSwitch = LIMIT_FR_GetValue();
                    leftSwitch = LIMIT_FL_GetValue();				
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
                    rightSwitch = LIMIT_FR_GetValue();
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
                    leftSwitch = LIMIT_FL_GetValue();				
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
            case frontRight:
                DEMUX_ENABLE_SetLow();     //Enable demux
                FRONT_1BACK_SetLow(); //Enable front motors select line on the demux 
                I11_R_SetLow();     //Enable right motors
                I12_R_SetLow();
                I11_L_SetHigh();     //Disable Left motors
                I12_L_SetHigh();    
                break;
                
            case frontLeft:
                DEMUX_ENABLE_SetLow();     //Enable demux
                FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
                I11_L_SetLow();     //Enable Left motors
                I12_L_SetLow();     
                I11_R_SetHigh();     //Disable Right motors
                I12_R_SetHigh(); 
                break;
                
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
                
            case bothFront:
                DEMUX_ENABLE_SetLow();     //Enable demux
                FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
                I11_L_SetLow();
                I12_L_SetLow();
                I11_R_SetLow();     //Enable both left and right motors
                I12_R_SetLow(); 
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
