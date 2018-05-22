/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: systick.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Timing using the SysTick timer. An interrupt is triggered
*              every 1 millisecond, which increments the global variable
*              'ticks'.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

#ifndef SYSTICK_H
#define SYSTICK_H


/***************************** Include files *******************************/
#include <stdint.h>
#include "emp_type.h"

/*****************************    Defines    *******************************/
#define MS_PER_TICK 5 // Milliseconds per tick
#define MILLISEC(ms) (ms / MS_PER_TICK) // Convert milliseconds to number of ticks

/********************** External declaration of Variables ******************/
extern volatile INT16U ticks;

/*****************************   Constants   *******************************/

/*************************  Function interfaces ****************************/

void enable_global_int(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Enable global interrupts.
******************************************************************************/

void disable_global_int(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Disable global interrupts.
******************************************************************************/

void systick_init(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Initialization of the SysTick timer. An interrupt is triggered
*							 every 5 ms.
******************************************************************************/

extern void systick_isr(void);
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Interrupt service routine for the SysTick timer. Increments the
*							 ticks variable.
*							 IMPORTANT: Remember to add this function to the startup file!
******************************************************************************/

void us_delay(INT16U ms);

/****************************** End Of Module *******************************/

#endif
