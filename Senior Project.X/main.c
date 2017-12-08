//Include Files
#include "mcc_generated_files/mcc.h"
#include "Motor.h"
#include "main.h"



//Function Prototypes
void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void);   //Timer 1 Interrupt Function
void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void);   //Timer 2/3 Interrupt Function
void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt (void);  //Timer 4/5 Interrupt Function  (5ms timer)
void initialize_5ms_Timer(void);
void motorTest(void);
void timer23Init(void);
void takeDown(void);    //Function when the take down switch is hit, retracts all legs
void setUp(void);   //Function when "Set up" button is pressed

//Global Variables
bool forward = true;   //Bool used to let the controller know which way the motor should move. (forward or reverse)
bool leftM = false;  //Bool used to let the controller know which motor to move (true = left motor only or false = right motor only) 
bool bothM = false;  //Bool used to let the controller know which motor to move (true = move left and right or false = move single motor)
int motorStage = 1; // integer for switch statement in Timer1 Interrupt function
bool timer23Flag = false;   //Flag used to control speed of motor
int wait = 0; //wait var used for back-off function
bool rightLow = false;
bool rightHigh = false;
bool leftHigh = false;      //Initial conditions for limit switch flags 
bool leftLow  = false;


int main(void)
{    
    // initialize the device
    SYSTEM_Initialize(); 
    
    initialize_5ms_Timer();
    
    
    timer23Init();
    //motorTest();    
    
    
    
    while (1)
    {   
        if(SETUP_GetValue() == 0)
        {
            int tiltBackLeft = TILT_BACK_LEFT_GetValue();
            int tiltBackRight = TILT_BACK_RIGHT_GetValue();
            int tiltRightLow = TILT_RIGHT_LOW_GetValue();
            int tiltRightHigh = TILT_RIGHT_HIGH_GetValue();
            int tiltLeftHigh = TILT_LEFT_HIGH_GetValue();
            int tiltLeftLow = TILT_LEFT_LOW_GetValue();
            setUp();            
        }
        
        if(TAKEDOWN_GetValue() == 0)  //If Take Down button is pushed run the following code
        {
            takeDown();
        }
        
        
    }

    return -1;
}

void __attribute__((__interrupt__, __auto_psv__)) _T1Interrupt(void)  
{ 
    if(timer23Flag)
    {
        if (forward)    //Check if motors are forward or reverse direction
        {  
            if(bothM)   //Check if both motors should be driven
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
            else //Only Left or right motor should be moving
            {
                if(leftM) //Only Left motor should be driven
                {
                    switch(motorStage)
                    {
                        case 1:            
                            PHASE1_L_SetLow(); //Phase1 Left                       
                            motorStage++;
                            break;
                        case 2:
                            PHASE2_L_SetLow();//Phase2 Left                        
                            motorStage++;
                            break;
                        case 3:
                            PHASE1_L_SetHigh(); //Phase1 Left                        
                            motorStage++;
                            break;
                        case 4:
                            PHASE2_L_SetHigh();//Phase2 Left                        
                            motorStage = 1;
                            break;
                        default:
                            motorStage = 1;
                            break;
                    }
                }
                else //Only right motor should be driven
                {
                    switch(motorStage)
                    {
                        case 1:                           
                            PHASE1_R_SetLow(); //Phase1 Right
                            motorStage++;
                            break;
                        case 2:
                            PHASE2_R_SetLow();//Phase2 right
                            motorStage++;
                            break;
                        case 3:
                            PHASE1_R_SetHigh(); //Phase1 Right
                            motorStage++;
                            break;
                        case 4:
                            PHASE2_R_SetHigh();//Phase2 Right
                            motorStage = 1;
                            break;
                        default:
                            motorStage = 1;
                            break;
                    }
                }
            }
            
        }    
        else    //Move motors in "Reverse" direction
        {
            if(bothM) //Check if both motors should be driven
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
            else //Only Left or right motor should be moving
            {
               if(leftM) //Only Left motor should be driven
               {
                   switch(motorStage)
                    {
                        case 1:            
                            PHASE2_L_SetLow(); //Phase1 Left                            
                            motorStage++;
                            break;
                        case 2:
                            PHASE1_L_SetLow();//Phase2 Left                            
                            motorStage++;
                            break;
                        case 3:
                            PHASE2_L_SetHigh(); //Phase1 Left                            
                            motorStage++;
                            break;
                        case 4:
                            PHASE1_L_SetHigh();//Phase2 Left                            
                            motorStage = 1;
                            break;
                        default:
                            motorStage = 1;
                            break;
                    }
               }
               else //Only right motor should be driven
               {
                   switch(motorStage)
                    {
                        case 1:
                            PHASE2_R_SetLow(); //Phase1 Right
                            motorStage++;
                            break;
                        case 2:
                            PHASE1_R_SetLow();//Phase2 right
                            motorStage++;
                            break;
                        case 3:
                            PHASE2_R_SetHigh(); //Phase1 Right
                            motorStage++;
                            break;
                        case 4:
                            PHASE1_R_SetHigh();//Phase2 Right
                            motorStage = 1;
                            break;
                        default:
                            motorStage = 1;
                            break;
                    } 
               }
            }
        }
        timer23Flag = false; //Reset Timer Flag
        wait++; //Increment wait Var
    }    
   _T1IF = 0;   //Reset interrupt flag.
}

void __attribute__((__interrupt__, __auto_psv__)) _T3Interrupt(void)  
{   
   timer23Flag = true; //set flag to true to trigger motor to drive   
   _T3IF = 0;   //Reset interrupt flag.
}

void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt(void) 
{
    bool backLeftS =  LIMIT_BL_GetValue();
    bool backRightS = LIMIT_BR_GetValue();
    
    if(!backLeftS) //Check if back left limit switch was hit
    {
        if (forward)
        {
            leftHigh = true; //High left limit was hit
            leftLow = false; 
        }
        if (!forward)
        {
            leftLow = true; //low left limit was hit
            leftHigh = false;
        }
    }
    
    if(!backRightS) //Check if back right limit switch was hit
    {
        if (forward)
        {
            rightHigh = true;   //High right limit was hit
            rightLow = false;   
        }
        if (!forward)
        {
            rightLow = true;    //low right limit was hit
            rightHigh = false;   
        }
    }
    
    _T5IF = 0;   //Reset interrupt flag.
}

void motorTest(void)
{       
    //extendMotor(frontRight);
    //extendMotor(backLeft);
    //retractAllMotors();
    //enableMotor(backRight);
    retractMotor(backRight);
    //extendFrontRightMotor();
}

void timer23Init(void)
{
  TMR3 = 0x0000;  //Clear timer 3  
  //PR3 = 0x04C4;   //Period for most significant word of (time) 5s timer
  //PR3 = 0x00F4;   //Period for most significant word of (time) 1s timer
  //PR3 = 0x003D;   //Period for most significant word of (time) 0.25s timer
  //PR3 = 0x0009;   //Period for most significant word of (time) 0.04s timer
  //PR3 = 0x0003;   //Period for most significant word of (time) 0.014s timer
  //PR3 = 0x0001;   //Period for most significant word of (time) 0.005s timer (8mm/s)
  PR3 = 0x0002;   //Period for most significant word of (time) 0.005s timer (4mm/s)
  TMR2 = 0;       //Clear timer 2
  _T3IP = 3;      //Priority bit for timer
  //PR2 = 0xB400-1;     //Period for least significant word of (time) 5s timer      (0.008mm/s)
  //PR2 = 0x2400-1;     //Period for least significant word of (time) 1s timer      (0.04mm/s)
  //PR2 = 0x0900-1;     //Period for least significant word of (time) 0.25s timer   (0.16mm/s)
  //PR2 = 0xC400-1;     //Period for least significant word of (time) 0.04s timer   (1mm/s)
  //PR2 = 0x6B00-1;     //Period for least significant word of (time) 0.014s timer  (2.86 mm/s)
  //PR2 = 0x3880-1;     //Period for least significant word of (time) 0.005s timer    (8mm/s)
  PR2 = 0x7100-1;     //Period for least significant word of (time) 0.005s timer    (4mm/s)
  T2CON = 0x8008; //TCKPS 1:1; T32 32 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TGATE disabled; 
  _T3IF = 0;  //Clear interrupt flag
  _T3IE = 1;  //Enable clock source      
}

void takeDown(void)
{    
    STATUS_LED_SetHigh();   //Turn status LED on to let user know the microcontroller is currently "Working".
    timer23Init();  //Initialize 32 bit 2/3 Timer
    retractMotor(backRight);
    retractMotor(backLeft);
    STATUS_LED_SetLow();    //Turn status LED off to let user know the microcontroller is done "Working".
    disableAllMotors();
}

void setUp(void)
{
    STATUS_LED_SetHigh();   //Turn status LED on to let user know the microcontroller is currently "Working".
    initialMotorExtend();
    levelHorizontally();
    
    STATUS_LED_SetLow();    //Turn status LED off to let user know the microcontroller is done "Working".
}

void initialize_5ms_Timer(void)
{
    TMR5 = 0x0000;  //Clear Timer 5
    PR5 = 0x0001;   //Period for most significant word of (time) (5ms timer)
    TMR4 = 0x0000;  //Clear Timer 4
    PR4 = 0x3880-1;   //Period for least significant word of (time) (5ms timer)
    _T5IP = 2;      //Priority bit for timer
    T4CON = 0x8008; //TCKPS 1:1; T32 32 Bit; TON enabled; TSIDL disabled; TCS FOSC/2; TECS SOSC; TGATE disabled;
    _T5IF = 0;  //Clear interrupt flag
    _T5IE = 1;  //Enable clock source
}

/**
 End of File
*/