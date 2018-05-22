#ifndef RTCS_H
#define RTCS_H

#include <stdint.h>
#include "emp_type.h"

#define NUM_TASKS 16 // Maximum number of tasks

typedef struct // Task Control Block
{
    INT8U condition; // e.g. READY, WAITING_FOR_SEM, etc.
    INT8U name;
    INT8U state; // State of the state machine (to pass into the task function)
    INT8U event;
    INT8U sem;
    INT16U timer;
    void (*task_function)(INT8U task_id, INT8U state, INT8U event, INT8U data); // Pointer to the tasks function
    // task_id is not actually used for anything. Might be useful later?
} TCB_T;

void rtcs_init(void);
void rtcs_schedule(void);
INT8U rtcs_acquire_id(void);
void rtcs_start_task(INT8U task_name, void (*task_function)(INT8U task_id, INT8U state, INT8U event, INT8U data));
void rtcs_set_timeout(INT8U timeout);
void rtcs_set_state(INT8U state);
void rtcs_set_semaphore(INT8U semaphore);
void rtcs_wait_semaphore(INT8U semaphore);
TCB_T* rtcs_get_tasklist(void);

#endif
