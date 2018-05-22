#include "launchpad.h"
#include <stdint.h>
#include "emp_type.h"
#include "tm4c123gh6pm.h"

void launchpad_init(void)
{
    // Setup PORTF
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5;// Enable clock for PORTF
    GPIO_PORTF_LOCK_R = 0x4C4F434B;         // Unlock the CR register
    GPIO_PORTF_CR_R = 0xFF;                 // Enable overwrite of PUR to FP0
    GPIO_PORTF_DIR_R = 0b00001110;          // Set LED's as output
    GPIO_PORTF_DEN_R = 0b00011111;          // Digital enable for SW1, SW2 and LED's
    GPIO_PORTF_PUR_R = 0b00010001;          // Enable pull-up resistor for SW1 and SW2
    
    return;
}

void launchpad_rgb(INT8U red, INT8U green, INT8U blue)
{
    if (red)
    {
        GPIO_PORTF_DATA_R |= 0b00000010;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~(0b00000010);
    }

    if (green)
    {
        GPIO_PORTF_DATA_R |= 0b00001000;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~(0b00001000);
    }

    if (blue)
    {
        GPIO_PORTF_DATA_R |= 0b00000100;
    }
    else
    {
        GPIO_PORTF_DATA_R &= ~(0b00000100);
    }

    // Write bit pattern to LED bits on PORTF
    //GPIO_PORTF_DATA_R = ( (red << 1) | (blue << 2) | (green << 3) );
    
    return;
}
