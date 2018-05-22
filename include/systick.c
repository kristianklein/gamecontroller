/*****************************************************************************
* University of Southern Denmark
* Embedded Programming (EMP)
*
* MODULENAME.: systick.h
*
* PROJECT....: EMP
*
* DESCRIPTION: Timing using the SysTick timer. An interrupt is triggered
*              every 10 microsecond, which increments the global variable
*							 '_systick_us' variable by 10 each time through the ISR, and the
*							 '_systick_ms' variable every 100th time through the ISR.
*
* Change Log:
******************************************************************************
* Date    Id    Change
* YYMMDD
* --------------------
* 050128  KA    Module created.
*
*****************************************************************************/

/***************************** Include files *******************************/
#include "systick.h"
#include "tm4c123gh6pm.h"

/*****************************    Defines    *******************************/
#define MS_PER_TICK 5
#define CLOCKS_PER_MS 16000 // Core clock 16 MHz
#define CLOCKS_PER_US (CLOCKS_PER_MS/1000)
#define SYSTICK_RELOAD_VALUE (CLOCKS_PER_MS * MS_PER_TICK) // 24-bits (max. ~16 million)

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
volatile INT16U ticks = 0; // Declared volatile because it is changed by an ISR

/*****************************   Functions   *******************************/

void enable_global_int(void)
{
    __asm("cpsie i");
    
    return;
}

void disable_global_int(void)
{
    __asm("cpsid i");
    
    return;
}

void systick_init(void)
{
    disable_global_int();
    NVIC_ST_CTRL_R &= ~(NVIC_ST_CTRL_ENABLE); // Disable SysTick
    NVIC_ST_RELOAD_R = SYSTICK_RELOAD_VALUE; // Set RELOAD value
    NVIC_ST_CURRENT_R = SYSTICK_RELOAD_VALUE; // Clear CURRENT register by writing any value to it
    NVIC_ST_CTRL_R |= (NVIC_ST_CTRL_CLK_SRC | NVIC_ST_CTRL_INTEN); // Set clock source to system clock and enable interrupt
    NVIC_ST_CTRL_R |= NVIC_ST_CTRL_ENABLE; // Enable SysTick
    enable_global_int();
    return;
}

void systick_isr(void)
{
    ticks++;
    
    return;
}

void us_delay(INT16U ms)
{
  for (INT32U i = 0; i < ms*CLOCKS_PER_US; i++);
  
  return;
}
