/*
 * File:   tmr4_5.c
 * Author: baske
 *
 * Created on December 7, 2017, 5:42 PM
 */


#include "xc.h"
#include "tmr4_5.h"




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

void __attribute__ ( ( interrupt, no_auto_psv ) ) _T5Interrupt (  )
{
    _T5IF = 0;   //Reset interrupt flag.
}