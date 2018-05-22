#ifndef BUTTON_H
#define BUTTON_H

#include <stdint.h>
#include "emp_type.h"

#define DOUBLECLICK_TIME 100
#define HOLD_TIME 2000
#define SW1 1
#define SW2 2
#define NUM_BUTTONS 2

// Function declarations
//void button_task(void);
BOOLEAN button_active(INT8U button_number);

#endif
