#ifndef SW_TIMER_T
#define SW_TIMER_T

#include <stdint.h>
#include "emp_type.h"

#define NUM_TIMERS 8 // Number of software timers

void sw_timer_init(void);
void sw_timer_update(void);
void start_sw_timer(INT8U timer_num, INT8U event_buffer, INT16U time);
BOOLEAN sw_timer_in_use(INT8U timer_num);

#endif
