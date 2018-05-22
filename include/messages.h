#ifndef MESSAGES_H
#define MESSAGES_H

#include <stdint.h>
#include "emp_type.h"

#define NUM_BUFFERS 64

INT8U get_event_msg(INT8U event_num);
void put_event_msg(INT8U event_num, INT8U event);

INT8U get_state_msg(INT8U state_num);
void put_state_msg(INT8U state_num, INT8U state);


#endif