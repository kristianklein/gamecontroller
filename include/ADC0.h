#ifndef ADC0_H_
#define ADC0_H_

#include <stdint.h>
#include "emp_type.h"
#include "tm4c123gh6pm.h"

void ADC0_init_seq3_ch1(void);
INT32U ADC0_read(void);
void ADC0_multiplex(INT8U pin);



#endif
