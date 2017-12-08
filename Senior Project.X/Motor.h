
/* 
 * File:  Motor.h 
 * Author: Kodey Boreo
 * Comments: Header file for Motor.c
 * Revision history: 
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef XC_HEADER_TEMPLATE_H
#define	XC_HEADER_TEMPLATE_H

#include <xc.h> // include processor files - each processor file is guarded. 
#include "mcc_generated_files/mcc.h"

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* XC_HEADER_TEMPLATE_H */

typedef enum
{
    backLeft, backRight, frontLeft, frontRight, bothFront, bothBack, left, right, both
}location;

void disableAllMotors(void);        //Disables all motors
void retractAllMotors(void);
void backOff(location loc);
void extendMotor(location loc);
void enableMotor(location loc);
void driveMotor(location loc);
void retractMotor(location loc);
void initialMotorExtend(void);
void levelHorizontally(void);
void raise10Degrees(void);
