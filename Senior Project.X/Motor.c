/*
 * File:   Motor.c
 * Author: kboreo
 *
 * Created on November 15, 2017, 4:39 PM
 */


#include "motor.h"
#include "main.h"


void enableBothFrontMotors(void)
{
    DEMUX_ENABLE_SetLow();     //Enable demux
    FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
    I11_L_SetLow();
    I12_L_SetLow();
    I11_R_SetLow();     //Enable both left and right motors
    I12_R_SetLow();    
}

void enableLeftFrontMotor(void)
{
    DEMUX_ENABLE_SetLow();     //Enable demux
    FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
    I11_L_SetLow();     //Enable Left motors
    I12_L_SetLow();     
    I11_R_SetHigh();     //Disable Right motors
    I12_R_SetHigh();    
}

void enableRightFrontMotor(void)
{
    DEMUX_ENABLE_SetLow();     //Enable demux
    FRONT_1BACK_SetLow(); //Enable front motors select line on the demux 
    I11_R_SetLow();     //Enable right motors
    I12_R_SetLow();
    I11_L_SetHigh();     //Disable Left motors
    I12_L_SetHigh();
    
}

void enableBothRearMotors(void)
{
    DEMUX_ENABLE_SetLow();  //Enable demux
    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
    I11_L_SetLow();
    I12_L_SetLow();
    I11_R_SetLow();     //Enable both left and right motors
    I12_R_SetLow();    
}

void enableLeftRearMotor(void)
{
    DEMUX_ENABLE_SetLow();  //Enable demux
    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
    I11_L_SetLow();     //Enable left motors
    I12_L_SetLow();
    I11_R_SetHigh();     //Disable Right motors
    I12_R_SetHigh();
    
}

void enableRightRearMotor(void)
{
    DEMUX_ENABLE_SetLow();  //Enable demux
    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux
    I11_R_SetLow();     //Enable right motors
    I12_R_SetLow();
    I11_L_SetHigh();     //Disable Left motors
    I12_L_SetHigh();
}

void disableAllMotors(void)
{
    T1CON = 0x0; //disable timer1
    I11_L_SetHigh();
    I12_L_SetHigh();        //Disables all motors 
    I11_R_SetHigh();    
    I12_R_SetHigh();
}

void retractAllMotors()
{    
    retractFrontMotors();
    retractRearMotors();
}

void driveBothMotorsForward()
{
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
}

void driveBothMotorsReverse()
{
    forward = false; //Motors are driving in the "reverse" direction
    
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
}

void driveRightMotorReverse()
{
    forward = false; //Motors are driving in the "reverse" direction
    
    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
    TMR1 = 0; // Init the timer
    PR1 = 158080-1; // set the period register
    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
    _T1IF = 0; //Clear Interrupt Flag
    _T1IE = 1; //Enable Clock Source
    
    PHASE1_R_SetHigh(); //Phase1 Right Motors
    PHASE2_R_SetHigh(); //Phase2 Right Motors 
}

void driveLeftMotorReverse()
{
    forward = false; //Motors are driving in the "reverse" direction
    
    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
    TMR1 = 0; // Init the timer
    PR1 = 158080-1; // set the period register
    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
    _T1IF = 0; //Clear Interrupt Flag
    _T1IE = 1; //Enable Clock Source
    
    PHASE1_L_SetHigh(); //Phase1 Left Motors
    PHASE2_L_SetHigh(); //Phase2 Left Motors 
}

void retractFrontMotors()
{
    forward = false;
    volatile bool leftSwitch;   //bool for the limit switch on the left side
    volatile bool rightSwitch;  //bool for the limit switch on the right side
    //volatile bool leftFlag;     //flag when left limit switch has been hit to prevent leg moving again after limit switch was hit
    //volatile bool rightFlag;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
    rightSwitch = LIMIT_FR_GetValue();
    leftSwitch = LIMIT_FL_GetValue();
    
    while ((rightSwitch + leftSwitch) > 0)
	{
		if (rightSwitch && leftSwitch)
		{
			enableBothFrontMotors();
			driveBothMotorsReverse();

			while (rightSwitch && leftSwitch) 
			{
                rightSwitch = LIMIT_FR_GetValue();
                leftSwitch = LIMIT_FL_GetValue();				
			}
			disableAllMotors();
		}

		if (rightSwitch)
		{
			enableRightFrontMotor();
			driveRightMotorReverse();

			while (rightSwitch)
			{
				rightSwitch = LIMIT_FR_GetValue();
			}
			disableAllMotors();
		}

		if (leftSwitch)
		{
			enableLeftFrontMotor();
			driveLeftMotorReverse();

			while (leftSwitch)
			{                
                leftSwitch = LIMIT_FL_GetValue();				
			}
			disableAllMotors();
		}		
	}
    disableAllMotors();
}

void retractRearMotors()
{
    forward = false;
    volatile bool leftSwitch;   //bool for the limit switch on the left side
    volatile bool rightSwitch;  //bool for the limit switch on the right side
    //volatile bool leftFlag = false;     //flag when left limit switch has been hit to prevent leg moving again after limit switch was hit
    //volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
    rightSwitch = LIMIT_BR_GetValue();
    leftSwitch = LIMIT_BL_GetValue();
    
    while ((rightSwitch + leftSwitch) > 0)
	{
		if (rightSwitch && leftSwitch)
		{
			enableBothRearMotors();
			driveBothMotorsReverse();

			while (rightSwitch && leftSwitch) 
			{
                rightSwitch = LIMIT_BR_GetValue();
                leftSwitch = LIMIT_BL_GetValue();				
			}
			disableAllMotors();
		}

		if (rightSwitch)
		{                        
			enableRightRearMotor();
			driveRightMotorReverse();

			while (rightSwitch)
			{
				rightSwitch = LIMIT_BR_GetValue();
			}
			disableAllMotors();                        
		}

		if (leftSwitch)
		{
			enableLeftRearMotor();
			driveLeftMotorReverse();

			while (leftSwitch)
			{                
                leftSwitch = LIMIT_BL_GetValue();				
			}
			disableAllMotors();
		}		
	}
    disableAllMotors();
}

void driveRightMotors()
{   
    forward = false; //Motors are driving in the "reverse" direction    
    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
    TMR1 = 0; // Init the timer
    PR1 = 158080-1; // set the period register
    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
    _T1IF = 0; //Clear Interrupt Flag
    _T1IE = 1; //Enable Clock Source    
    PHASE1_R_SetHigh(); //Phase1 Right Motors
    PHASE2_R_SetHigh(); //Phase2 Right Motors 
}

void extendFrontRightMotor()
{   
    forward = true;
    volatile bool rightSwitch;  //bool for the limit switch on the right side    
    volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    rightSwitch = LIMIT_FR_GetValue();    
    
    if (rightSwitch)
	{
		enableRightFrontMotor();
		driveBothMotorsForward();
        
		while (rightSwitch) 
		{
            rightSwitch = LIMIT_FR_GetValue();            			
		}
        rightFlag = true;
        disableAllMotors();	
	}
    if(rightFlag)
    {
        backOff(frontRight);
    }
    disableAllMotors();
}

void backOff(location loc)
{
    forward = !forward;
    int target = 300;
    wait = 0;
    enableMotor(loc);
    
    if(loc == frontRight)
    {
        driveMotor(right);       	 
    }
    
    if(loc == backRight)
    {
        driveMotor(right);       	 
    }
    
    if(loc == frontLeft)
    {        
        driveMotor(left); 
    }
    
    if(loc == backLeft)
    {        
        driveMotor(left); 
    }
    
    if(loc == bothFront)
    {
         driveMotor(both);
    }
    
    if(loc == bothBack)
    {
         driveMotor(both);
    }
    
    while(wait <= target)
    {

    }
    disableAllMotors();
}

void extendMotor(location loc)
{
    if(loc == frontRight)
    {        
        forward = true;
        volatile bool rightSwitch;  //bool for the limit switch on the right side    
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        rightSwitch = LIMIT_FR_GetValue();    
    
        if (rightSwitch)
        {
            enableMotor(loc);
            driveMotor(right);

            while(rightSwitch) 
            {
                rightSwitch = LIMIT_FR_GetValue();            			
            }
            rightFlag = true;
            disableAllMotors();	
        }
        if(rightFlag)
        {
            backOff(loc);
        }
        disableAllMotors();
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
            driveMotor(left);

            while(leftSwitch) 
            {
                leftSwitch = LIMIT_FL_GetValue();            			
            }
            leftFlag = true;
            disableAllMotors();	
        }
        if(leftFlag)
        {
            backOff(loc);
        }
        disableAllMotors();
    }
    
    if(loc == backLeft)
    {
        forward = true;
        volatile bool leftSwitch;  //bool for the limit switch on the right side    
        volatile bool leftFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        leftSwitch = LIMIT_BL_GetValue();    
    
        if (leftSwitch)
        {
            enableMotor(loc);
            driveMotor(left);

            while(leftSwitch) 
            {
                leftSwitch = LIMIT_BL_GetValue();            			
            }
            leftFlag = true;
            disableAllMotors();	
        }
        if(leftFlag)
        {
            backOff(loc);
        }
        disableAllMotors();
    }
    
    if(loc == backRight)
    {
        forward = true;
        volatile bool rightSwitch;  //bool for the limit switch on the right side    
        volatile bool rightFlag = false;    //flag when right limit switch has been hit to prevent leg moving again after limit switch was hit
    
        rightSwitch = LIMIT_BR_GetValue();    
    
        if (rightSwitch)
        {
            enableMotor(loc);
            driveMotor(right);

            while(rightSwitch) 
            {
                rightSwitch = LIMIT_BR_GetValue();            			
            }
            rightFlag = true;
            disableAllMotors();	
        }
        if(rightFlag)
        {
            backOff(loc);
        }
        disableAllMotors();
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
                enableMotor(bothBack);
                driveMotor(both);

                while (rightSwitch && leftSwitch) 
                {
                    rightSwitch = LIMIT_FR_GetValue();
                    leftSwitch = LIMIT_FL_GetValue();				
                }
                leftFlag = true;
                rightFlag = true;                
                disableAllMotors();
            }
            
            if (leftFlag && rightFlag)
            {
                backOff(loc);
            }

            if (rightSwitch)
            {
                enableMotor(backRight);
                driveMotor(right);

                while (rightSwitch)
                {
                    rightSwitch = LIMIT_FR_GetValue();
                }
                rightFlag = true;
                disableAllMotors();                        
            }
            
            if (rightFlag)
                {
                    backOff(loc);
                }

            if (leftSwitch)
            {
                enableMotor(backLeft);
                driveMotor(left);

                while (leftSwitch)
                {                
                    leftSwitch = LIMIT_FL_GetValue();				
                }
                leftFlag = true;
                disableAllMotors();
            }
            
            if (leftFlag)
            {
                backOff(loc);
            }
        }
        disableAllMotors();
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
        
        rightSwitch = LIMIT_BR_GetValue();
        leftSwitch = LIMIT_BL_GetValue();

        while ((rightSwitch + leftSwitch) > 0)
        {
            if (rightSwitch && leftSwitch)
            {
                enableMotor(bothBack);
                driveMotor(both);

                while (rightSwitch && leftSwitch) 
                {
                    rightSwitch = LIMIT_BR_GetValue();
                    leftSwitch = LIMIT_BL_GetValue();				
                }
                leftFlag = true;
                rightFlag = true;                
                disableAllMotors();
            }
            
            if (leftFlag && rightFlag)
            {
                backOff(loc);
            }

            if (rightSwitch)
            {
                enableMotor(backRight);
                driveMotor(right);

                while (rightSwitch)
                {
                    rightSwitch = LIMIT_BR_GetValue();
                }
                rightFlag = true;
                disableAllMotors();                        
            }
            
            if (rightFlag)
                {
                    backOff(loc);
                }

            if (leftSwitch)
            {
                enableMotor(backLeft);
                driveMotor(left);

                while (leftSwitch)
                {                
                    leftSwitch = LIMIT_BL_GetValue();				
                }
                leftFlag = true;
                disableAllMotors();
            }
            
            if (leftFlag)
            {
                backOff(loc);
            }
        }
        disableAllMotors();
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
