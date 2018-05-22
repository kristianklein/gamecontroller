#include "ADC0.h"


void ADC0_init_seq3_ch1(void){
    volatile INT8U delay;
    SYSCTL_RCGC2_R |= 0x00000010;   // Activate clock for Port E.
    delay =  SYSCTL_RCGC2_R;        // Dummy read.

    GPIO_PORTE_DIR_R &= ~0x3E;      // Make PE2 input.
    GPIO_PORTE_AFSEL_R |= 0x3E;     // Enable alternate function on PE2 (Change from regular I/O port)
    GPIO_PORTE_DEN_R &= ~0x3E;      // Disable digital I/O on PE2 (Because we use analog input)
    GPIO_PORTE_AMSEL_R |= 0x3E;     // Enable analog function on PE2.

    SYSCTL_RCGC0_R |= 0x00010000;   // Activate ADC0.
    delay = SYSCTL_RCGC2_R;
    SYSCTL_RCGC0_R |= 0x00000200;   // Configure for 500k samples/sec.

    ADC0_SSPRI_R = 0x0123;          // Choose sequence 3 as highest priority.
    ADC0_ACTSS_R &= ~0x0008;        // Disable sample sequencer 3 (will get enabled at the end of init)
    ADC0_EMUX_R &= ~0xF000;         // Sequence 3 software trigger mode.
    ADC0_SSMUX3_R &= ~0x000F;       // Clear SS3

    ADC0_SSMUX3_R += 1;             // Use Ain1 (PE2)
    //ADC0_SSMUX3_R = (ADC0_SSMUX3_R&0xFFFFFFF0)+1;   // Choose channel 1 Ain1 (PE2)
    ADC0_SSCTL3_R = 0x0006;         // IE0 sample capture flag set to 1, TS0 is disabled (TS0 is a temperature sensor), set END0 bit (needs to be done before sampling)
    ADC0_ACTSS_R |= 0x0008;         // enable sample sequencer 3 again.
}

//  Busy-wait ADC
//  Input: none
//  Output: 12-bit result of ADC conversion.
INT32U ADC0_read(void)
{
    INT32U result;
    ADC0_PSSI_R = 0x0008;               // Begin sampling on Sample Sequence 3.
    while((ADC0_RIS_R & 0x08) == 0){};  // Wait for conversion to be done.
    result = ADC0_SSFIFO3_R & 0xFFF;    // Read result
    ADC0_ISC_R = 0x0008;                // Clear flag on IN3 (Read is complete)
    return result;
}

void ADC0_multiplex(INT8U Ain_nr)
{
    ADC0_SSMUX3_R  &= ~(0xF);
    ADC0_SSMUX3_R  |= Ain_nr;
    return;
}







