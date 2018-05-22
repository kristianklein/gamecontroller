#ifndef SYSBLINK_H
#define SYSBLINK_H

#include <stdint.h>
#include "emp_type.h"

void sysblink_init(void);
void sysblink_toggle_led(void);
void sysblink_task(INT8U task_id, INT8U state, INT8U event, INT8U data);

#endif
