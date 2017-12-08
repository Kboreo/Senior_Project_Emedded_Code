

/* 
 * File:   
 * Author: 
 * Comments:
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded.  


// TODO Insert declarations or function prototypes (right here) to leverage 
// live documentation

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

extern bool forward;
extern int wait;
extern bool leftM;   //Bool used to let the controller know which motor to move (true = left motor only or false = right motor only) 
extern bool bothM;  //Bool used to let the controller know which motor to move (true = move left and right or false = move single motor)
extern bool leftHigh;   //Flag that is turned high if upper limit switch is hit on the back left leg.
extern bool leftLow;   //Flag that is turned high if lower limit switch is hit on the back left leg.
extern bool rightHigh;   //Flag that is turned high if upper limit switch is hit on the back right leg.
extern bool rightLow;   //Flag that is turned high if lower limit switch is hit on the back right leg.
extern bool tiltBackLeft;   //TILT_BACK_LEFT_GetValue();
extern bool tiltBackRight;  //TILT_BACK_RIGHT_GetValue();
extern bool tiltRightLow;   //TILT_RIGHT_LOW_GetValue();
extern bool tiltRightHigh;  //TILT_RIGHT_HIGH_GetValue();          Condition Flags for rest of switches in system
extern bool tiltLeftHigh;   //TILT_LEFT_HIGH_GetValue();
extern bool tiltLeftLow;    //TILT_LEFT_LOW_GetValue();

extern void initialize_5ms_Timer(void);

extern void __attribute__((__interrupt__, __auto_psv__)) _T5Interrupt (void);  //Timer 4/5 Interrupt Function  (5ms timer)