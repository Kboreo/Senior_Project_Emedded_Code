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

bool forward;   //Bool used to let the controller know whether the motor should move forward or reverse
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
    forward = true;
    enableRightFrontMotor();
    
    //retractAllMotors(forward);
}


/**
 End of File
*/