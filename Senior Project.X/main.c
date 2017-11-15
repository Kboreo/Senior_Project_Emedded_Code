/**
  Generated Main Source File

  Company:
    Microchip Technology Inc.

  File Name:
    main.c

  Summary:
    This is the main file generated using PIC24 / dsPIC33 / PIC32MM MCUs

  Description:
    This header file provides implementations for driver APIs for all modules selected in the GUI.
    Generation Information :
        Product Revision  :  PIC24 / dsPIC33 / PIC32MM MCUs - pic24-dspic-pic32mm : v1.35
        Device            :  PIC24FJ32GA004
    The generated drivers are tested against the following:
        Compiler          :  XC16 1.31
        MPLAB             :  MPLAB X 3.60
*/

#include "mcc_generated_files/mcc.h"
#include "Motor.h"

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void motorTest(void);
//void enableBothFrontMotors(void);
//void enableLeftFrontMotor(void);
//void enableRightFrontMotor(void);
//void enableBothRearMotors(void);
//void enableLeftRearMotor(void);
//void enableRightRearMotor(void);
//void disableAllMotors(void);        //Disables all motors
//void driveBothMotorsReverse(void);
//void driveBothMotorsForward(void);
//void retractAllMotors(void);
//void driveLeftMotorReverse(void);
//void driveRightMotorReverse(void);
//void retractFrontMotors(void);
//void retractRearMotors(void);

bool forward;   //Bool used to let the controller know wheither the motor should move forward or reverse
int motorStage = 1; // integer for switch statement in Timer1 Interrupt function
int wait = 0; //wait var used for testing/debugging
bool limitSwitch;   //Variable used to test limit switch
/*
                         Main application
 */
int main(void)
{
    
    // initialize the device
    SYSTEM_Initialize();
    //T1CON = 0x8030; // 0b10000000_00110000 TMR1 on, prescaler 1:256 Tclk/2
    T1CON = 0x8000; // 0b10000000_00000000 TMR1 on, prescaler 1:1 Tclk/2
    //T1CON = 0x8020; // 0b10000000_00000000 TMR1 on, prescaler 1:64 Tclk/2
    
    motorTest();
    //disableAllMotors();
    
    while (1)
    {
        
        
    }

    return -1;
    
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)  
{ 
    if (forward)
    {        
        switch(motorStage)
        {
            case 1:            
                PHASE1_L_SetLow(); //Phase1 Left
                PHASE1_R_SetLow(); //Phase1 Right
                motorStage++;
                break;
            case 2:
                PHASE2_L_SetLow();//Phase2 Left
                PHASE2_R_SetLow();//Phase2 right
                motorStage++;
                break;
            case 3:
                PHASE1_L_SetHigh(); //Phase1 Left
                PHASE1_R_SetHigh(); //Phase1 Right
                motorStage++;
                break;
            case 4:
                PHASE2_L_SetHigh();//Phase2 Left
                PHASE2_R_SetHigh();//Phase2 Right
                motorStage = 1;
                break;
            default:
                motorStage = 1;
                break;
        }
    }    
    else
    {
        switch(motorStage)
        {
            case 1:            
                PHASE2_L_SetLow(); //Phase1 Left
                PHASE2_R_SetLow(); //Phase1 Right
                motorStage++;
                break;
            case 2:
                PHASE1_L_SetLow();//Phase2 Left
                PHASE1_R_SetLow();//Phase2 right
                motorStage++;
                break;
            case 3:
                PHASE2_L_SetHigh(); //Phase1 Left
                PHASE2_R_SetHigh(); //Phase1 Right
                motorStage++;
                break;
            case 4:
                PHASE1_L_SetHigh();//Phase2 Left
                PHASE1_R_SetHigh();//Phase2 Right
                motorStage = 1;
                break;
            default:
                motorStage = 1;
                break;
        }
    }
    wait++;     //Increment wait Var    
   _T1IF = 0;   //Reset interrupt flag.
}

void motorTest(void)
{
//    enableRightFrontMotor();    
//    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
//    TMR1 = 0; // Init the timer
//    PR1 = 158080-1; // set the period register
//    //PR1 = 158080-1;
//    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
//    _T1IF = 0; //Clear Interrupt Flag
//    _T1IE = 1; //Enable Clock Source
// 
//    forward = true;
//    PHASE1_L_SetHigh(); //Phase1 Left Motors
//    PHASE2_L_SetHigh(); //Phase2 Left Motors
//    
//    PHASE1_L_SetHigh(); //Phase1 Left Motors
//    PHASE2_L_SetHigh(); //Phase2 Left Motors
//    
//    //STATUS_LED_SetHigh();
//    
//    
//    while(1)
//    {   
//        limitSwitch = LIMIT_FR_GetValue();
//        if(!limitSwitch) //Checks if wait var has has been incremented 1012 times (aka approx 10 seconds have passed)
//        {
//            if(wait > 1000)
//            {    
//            forward = !forward;
//            wait = 0;
//            }
//        }        
//    }    
    
    retractAllMotors(forward);
}

//void enableBothFrontMotors(void)
//{
//    DEMUX_ENABLE_SetLow();     //Enable demux
//    FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
//    I11_L_SetLow();
//    I12_L_SetLow();
//    I11_R_SetLow();     //Enable both left and right motors
//    I12_R_SetLow();    
//}
//
//void enableLeftFrontMotor(void)
//{
//    DEMUX_ENABLE_SetLow();     //Enable demux
//    FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
//    I11_L_SetLow();     //Enable Left motors
//    I12_L_SetLow();     
//    I11_R_SetHigh();     //Disable Right motors
//    I12_R_SetHigh();    
//}
//
//void enableRightFrontMotor(void)
//{
//    DEMUX_ENABLE_SetLow();     //Enable demux
//    FRONT_1BACK_SetLow(); //Enable front motors select line on the demux 
//    I11_R_SetLow();     //Enable right motors
//    I12_R_SetLow();
//    I11_L_SetHigh();     //Disable Left motors
//    I12_L_SetHigh();
//    
//}
//
//void enableBothRearMotors(void)
//{
//    DEMUX_ENABLE_SetLow();  //Enable demux
//    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
//    I11_L_SetLow();
//    I12_L_SetLow();
//    I11_R_SetLow();     //Enable both left and right motors
//    I12_R_SetLow();    
//}
//
//void enableLeftRearMotor(void)
//{
//    DEMUX_ENABLE_SetLow();  //Enable demux
//    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
//    I11_L_SetLow();     //Enable left motors
//    I12_L_SetLow();
//    I11_R_SetHigh();     //Disable Right motors
//    I12_R_SetHigh();
//    
//}
//
//void enableRightRearMotor(void)
//{
//    DEMUX_ENABLE_SetLow();  //Enable demux
//    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux
//    I11_R_SetLow();     //Enable right motors
//    I12_R_SetLow();
//    I11_L_SetHigh();     //Disable Left motors
//    I12_L_SetHigh();
//}
//
//void disableAllMotors(void)
//{
//    T1CON = 0x0; //disable timer1
//    I11_L_SetHigh();
//    I12_L_SetHigh();        //Disables all motors 
//    I11_R_SetHigh();    
//    I12_R_SetHigh();
//}
//
//
//void retractAllMotors(void)
//{    
//    retractFrontMotors();
//    retractRearMotors();
//}
//
//void driveBothMotorsForward(void)
//{
//    forward = true; //Motors are driving in the "forward" direction
//    
//    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
//    TMR1 = 0; // Init the timer
//    PR1 = 158080-1; // set the period register
//    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
//    _T1IF = 0; //Clear Interrupt Flag
//    _T1IE = 1; //Enable Clock Source
//    
//    PHASE1_L_SetHigh(); //Phase1 Left Motors
//    PHASE2_L_SetHigh(); //Phase2 Left Motors    
//    PHASE1_R_SetHigh(); //Phase1 Right Motors
//    PHASE2_R_SetHigh(); //Phase2 Right Motors
//}
//
//void driveBothMotorsReverse(void)
//{
//    forward = false; //Motors are driving in the "reverse" direction
//    
//    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
//    TMR1 = 0; // Init the timer
//    PR1 = 158080-1; // set the period register
//    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
//    _T1IF = 0; //Clear Interrupt Flag
//    _T1IE = 1; //Enable Clock Source
//    
//    PHASE1_L_SetHigh(); //Phase1 Left Motors
//    PHASE2_L_SetHigh(); //Phase2 Left Motors    
//    PHASE1_R_SetHigh(); //Phase1 Right Motors
//    PHASE2_R_SetHigh(); //Phase2 Right Motors
//}
//
//void driveRightMotorReverse(void)
//{
//    forward = false; //Motors are driving in the "reverse" direction
//    
//    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
//    TMR1 = 0; // Init the timer
//    PR1 = 158080-1; // set the period register
//    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
//    _T1IF = 0; //Clear Interrupt Flag
//    _T1IE = 1; //Enable Clock Source
//    
//    PHASE1_R_SetHigh(); //Phase1 Right Motors
//    PHASE2_R_SetHigh(); //Phase2 Right Motors 
//}
//
//void driveLeftMotorReverse(void)
//{
//    forward = false; //Motors are driving in the "reverse" direction
//    
//    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
//    TMR1 = 0; // Init the timer
//    PR1 = 158080-1; // set the period register
//    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
//    _T1IF = 0; //Clear Interrupt Flag
//    _T1IE = 1; //Enable Clock Source
//    
//    PHASE1_L_SetHigh(); //Phase1 Left Motors
//    PHASE2_L_SetHigh(); //Phase2 Left Motors 
//}
//
//void retractFrontMotors(void)
//{
//    volatile bool leftSwitch;
//    volatile bool rightSwitch;
//    
//    rightSwitch = LIMIT_FR_GetValue();
//    leftSwitch = LIMIT_FL_GetValue();
//    
//    while ((rightSwitch + leftSwitch) > 0)
//	{
//		if (rightSwitch && leftSwitch)
//		{
//			enableBothFrontMotors();
//			driveBothMotorsReverse();
//
//			while (rightSwitch && leftSwitch) 
//			{
//                rightSwitch = LIMIT_FR_GetValue();
//                leftSwitch = LIMIT_FL_GetValue();				
//			}
//			disableAllMotors();
//		}
//
//		if (rightSwitch)
//		{
//			enableRightFrontMotor();
//			driveRightMotorReverse();
//
//			while (rightSwitch)
//			{
//				rightSwitch = LIMIT_FR_GetValue();
//			}
//			disableAllMotors();
//		}
//
//		if (leftSwitch)
//		{
//			enableLeftFrontMotor();
//			driveLeftMotorReverse();
//
//			while (leftSwitch)
//			{
//                rightSwitch = LIMIT_FR_GetValue();
//                leftSwitch = LIMIT_FL_GetValue();				
//			}
//			disableAllMotors();
//		}		
//	}
//    disableAllMotors();
//}
//
//void retractRearMotors(void)
//{
//    volatile bool leftSwitch;
//    volatile bool rightSwitch;
//    
//    rightSwitch = LIMIT_BR_GetValue();
//    leftSwitch = LIMIT_BL_GetValue();
//    
//    while ((rightSwitch + leftSwitch) > 0)
//	{
//		if (rightSwitch && leftSwitch)
//		{
//			enableBothRearMotors();
//			driveBothMotorsReverse();
//
//			while (rightSwitch && leftSwitch) 
//			{
//                rightSwitch = LIMIT_BR_GetValue();
//                leftSwitch = LIMIT_BL_GetValue();				
//			}
//			disableAllMotors();
//		}
//
//		if (rightSwitch)
//		{
//			enableRightRearMotor();
//			driveRightMotorReverse();
//
//			while (rightSwitch)
//			{
//				rightSwitch = LIMIT_BR_GetValue();
//			}
//			disableAllMotors();
//		}
//
//		if (leftSwitch)
//		{
//			enableLeftRearMotor();
//			driveLeftMotorReverse();
//
//			while (leftSwitch)
//			{
//                rightSwitch = LIMIT_BR_GetValue();
//                leftSwitch = LIMIT_BL_GetValue();				
//			}
//			disableAllMotors();
//		}		
//	}
//    disableAllMotors();
//}
/**
 End of File
*/