#ifndef QUEUE_H
#define QUEUE_H

#include <stdint.h>
#include "emp_type.h"

#define MAX_QUEUES 12 // Maximum number of queues
#define MAX_ELEMS 128 // Maximum number of elements in each queue

BOOLEAN queue_open(INT8U queue_number, INT8U size);
BOOLEAN queue_enqueue(INT8U queue_number, INT16U value);
BOOLEAN queue_enqueue_force(INT8U queue_number, INT16U value);
INT16U queue_dequeue(INT8U queue_number);
INT16U queue_peek(INT8U queue_number);
INT8U queue_capacity(INT8U queue_number);
BOOLEAN queue_is_empty(INT8U queue_number);
BOOLEAN queue_is_full(INT8U queue_number);
INT8U queue_increment(INT8U queue_number, INT8U head_or_tail);
INT8U queue_sem(INT8U queue_number);
INT8U queue_tail(INT8U queue_number);
INT8U queue_head(INT8U queue_number);


#endif
