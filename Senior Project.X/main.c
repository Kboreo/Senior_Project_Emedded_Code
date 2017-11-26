
#include "mcc_generated_files/mcc.h"
#include "Motor.h"
#include "main.h"

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);
void motorTest(void);


bool forward = true;   //Bool used to let the controller know which way the motor should move. (forward or reverse)
int motorStage = 1; // integer for switch statement in Timer1 Interrupt function
int wait = 0; //wait var used for testing/debugging

int main(void)
{    
    // initialize the device
    SYSTEM_Initialize(); 
    
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
    extendMotor(frontRight);
    extendMotor(frontLeft);
    retractAllMotors();
    
    //extendFrontRightMotor();
}


/**
 End of File
*/