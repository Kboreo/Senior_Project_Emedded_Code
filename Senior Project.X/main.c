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

/*
    (c) 2016 Microchip Technology Inc. and its subsidiaries. You may use this
    software and any derivatives exclusively with Microchip products.

    THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
    EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
    WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
    PARTICULAR PURPOSE, OR ITS INTERACTION WITH MICROCHIP PRODUCTS, COMBINATION
    WITH ANY OTHER PRODUCTS, OR USE IN ANY APPLICATION.

    IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
    INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
    WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
    BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
    FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
    ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
    THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.

    MICROCHIP PROVIDES THIS SOFTWARE CONDITIONALLY UPON YOUR ACCEPTANCE OF THESE
    TERMS.
*/

#include "mcc_generated_files/mcc.h"

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void motorTest(void);
void enableBothFrontMotors(void);
void enableLeftFrontMotor(void);
void enableRightFrontMotor(void);
void enableBothRearMotors(void);
void enableLeftRearMotor(void);
void enableRightRearMotor(void);
void disableAllMotors(void);        //Disables all motors

bool forward;   //Bool used to let the controller know wheither the motor should move forward or reverse
int motorStage = 1; // integer for switch statement in Timer1 Interrupt function
int wait = 0; //wait var used for testing/debugging 
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
    enableRightFrontMotor();
    
    
    _T1IP = 1; // this is the default value anyway, priority of Interrupt for Timer1
    TMR1 = 0; // Init the timer
    PR1 = 158080-1; // set the period register
    //PR1 = 158080-1;
    T1CON = 0x8000; // enabled, prescaler 1:1, internal clock
    _T1IF = 0; //Clear Interrupt Flag
    _T1IE = 1; //Enable Clock Source
 
    forward = true;
    PHASE1_L_SetHigh(); //Phase1 Left Motors
    PHASE2_L_SetHigh(); //Phase2 Left Motors
    
    PHASE1_L_SetHigh(); //Phase1 Left Motors
    PHASE2_L_SetHigh(); //Phase2 Left Motors
    
    //STATUS_LED_SetHigh();
    
    while(1)
    {        
        if(wait >= 5000) //Checks if wait var has has been incremented 1012 times (aka approx 10 seconds have passed)
        {
            forward = false;                   
            if(wait >= 10000)
            {             
                disableAllMotors();
                
                while(1)
                {
                //loop forever
                }
            }
        }        
    }    
}

void enableBothFrontMotors(void)
{
    DEMUX_ENABLE_SetLow();     //Enable demux
    FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
    I11_L_SetLow();
    I12_L_SetLow();
    I12_R_SetLow();     //Enable both left and right motors
    I12_R_SetLow();    
}

void enableLeftFrontMotor(void)
{
    DEMUX_ENABLE_SetLow();     //Enable demux
    FRONT_1BACK_SetLow(); //Enable front motors on the select line on the demux 
    I11_L_SetLow();     //Enable Left motors
    I12_L_SetLow();     
}

void enableRightFrontMotor(void)
{
    DEMUX_ENABLE_SetLow();     //Enable demux
    FRONT_1BACK_SetLow(); //Enable front motors select line on the demux 
    I12_R_SetLow();     //Enable right motors
    I12_R_SetLow();
}

void enableBothRearMotors(void)
{
    DEMUX_ENABLE_SetLow();  //Enable demux
    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
    I11_L_SetLow();
    I12_L_SetLow();
    I12_R_SetLow();     //Enable both left and right motors
    I12_R_SetLow();    
}

void enableLeftRearMotor(void)
{
    DEMUX_ENABLE_SetLow();  //Enable demux
    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux 
    I11_L_SetLow();     //Enable left motors
    I12_L_SetLow();
}

void enableRightRearMotor(void)
{
    DEMUX_ENABLE_SetLow();  //Enable demux
    FRONT_1BACK_SetHigh();  //Enable rear motors select line on the demux
    I12_R_SetLow();     //Enable right motors
    I12_R_SetLow(); 
}

void disableAllMotors(void)
{
    T1CON = 0x0; //disable timer1
    I11_L_SetHigh();
    I12_L_SetHigh();        //Disables all motors 
    I11_R_SetHigh();    
    I12_R_SetHigh();
}
/**
 End of File
*/