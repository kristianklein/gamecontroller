#ifndef SEMAPHORE_H
#define SEMAPHORE_H

#include <stdint.h>
#include "emp_type.h"

// Number of semaphores MUST be larger than number of queues,
// since 1 semaphore is reserved for each queue.
#define NUM_SEMAPHORES 64

BOOLEAN sem_wait(INT8U sem_number);
void sem_signal(INT8U sem_number);
void sem_set_value(INT8U sem_number, INT8U value);

#endif