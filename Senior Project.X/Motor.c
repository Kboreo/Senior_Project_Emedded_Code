/*
 * File:   Motor.c
 * Author: kboreo
 *
 * Created on November 15, 2017, 4:39 PM
 */


#include "motor.h"


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


void retractAllMotors(bool forward)
{    
    retractFrontMotors(forward);
    retractRearMotors(forward);
}

void driveBothMotorsForward(bool forward)
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

void driveBothMotorsReverse(bool forward)
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

void driveRightMotorReverse(bool forward)
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

void driveLeftMotorReverse(bool forward)
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

void retractFrontMotors(bool forward)
{
    volatile bool leftSwitch;
    volatile bool rightSwitch;
    
    rightSwitch = LIMIT_FR_GetValue();
    leftSwitch = LIMIT_FL_GetValue();
    
    while ((rightSwitch + leftSwitch) > 0)
	{
		if (rightSwitch && leftSwitch)
		{
			enableBothFrontMotors();
			driveBothMotorsReverse(forward);

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
			driveRightMotorReverse(forward);

			while (rightSwitch)
			{
				rightSwitch = LIMIT_FR_GetValue();
			}
			disableAllMotors();
		}

		if (leftSwitch)
		{
			enableLeftFrontMotor();
			driveLeftMotorReverse(forward);

			while (leftSwitch)
			{
                rightSwitch = LIMIT_FR_GetValue();
                leftSwitch = LIMIT_FL_GetValue();				
			}
			disableAllMotors();
		}		
	}
    disableAllMotors();
}

void retractRearMotors(bool forward)
{
    volatile bool leftSwitch;
    volatile bool rightSwitch;
    
    rightSwitch = LIMIT_BR_GetValue();
    leftSwitch = LIMIT_BL_GetValue();
    
    while ((rightSwitch + leftSwitch) > 0)
	{
		if (rightSwitch && leftSwitch)
		{
			enableBothRearMotors();
			driveBothMotorsReverse(forward);

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
			driveRightMotorReverse(forward);

			while (rightSwitch)
			{
				rightSwitch = LIMIT_BR_GetValue();
			}
			disableAllMotors();
		}

		if (leftSwitch)
		{
			enableLeftRearMotor();
			driveLeftMotorReverse(forward);

			while (leftSwitch)
			{
                rightSwitch = LIMIT_BR_GetValue();
                leftSwitch = LIMIT_BL_GetValue();				
			}
			disableAllMotors();
		}		
	}
    disableAllMotors();
}
