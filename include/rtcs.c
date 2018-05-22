#include "rtcs.h"
#include "semaphore.h"
#include "queue.h"
#include "taskmodel.h"
#include "tasks.h"
#include "systick.h"
#include "sysblink.h"
#include "swtimer.h"
#include "tm4c123gh6pm.h"
#include "launchpad.h"

#define USER_TASK 1 // Zero'th task is reserved for sysblink
#define USER_QUEUE 0 // All queues available to the user
#define USER_SEMAPHORES NUM_QUEUES // Reserve first NUM_QUEUES elements for the queues

/* Moved to header, so tasks know the type
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
*/
enum CONDITIONS
{
	COND_NO_TASK,
	COND_READY,
	COND_WAIT_TIMEOUT,
	COND_WAIT_SEMAPHORE,
	COND_WAIT_FOR_EVENT,
};

TCB_T tasks[NUM_TASKS];
INT8U current_task;

void rtcs_init(void)
{
	// Execute init functions (remember to setup ISR for systick!)
	systick_init();
	sysblink_init();
	sw_timer_init();

	// Initialize task conditions to COND_NO_TASK
	for (INT8U i = 0; i < NUM_TASKS; i++)
	{
		tasks[i].condition = COND_NO_TASK;
	}
	
	// Start sysblink task
	rtcs_start_task(TASK_SYSBLINK, sysblink_task);
	
	return;
}

void rtcs_schedule(void)
{
	while (1)
	{
	    // Wait for tick
	    while (!ticks);
	    ticks--;

	    // Update software timers
	    sw_timer_update();
        // Loop through tasks in task list
        for (current_task = 0; current_task < NUM_TASKS; current_task++)
        {
            // End of task list - break out of loop
            if (tasks[current_task].condition == COND_NO_TASK)
            {
                break;
            }

            // If task is waiting for semaphore, check if it has become available
            if (tasks[current_task].condition == COND_WAIT_SEMAPHORE)
            {
                if (sem_wait(tasks[current_task].sem))
                {
                    tasks[current_task].condition = COND_READY;
                    tasks[current_task].event = SE_SIGNAL;
                }
            }

            // If task is waiting for timeout, check if it has occurred
            if (tasks[current_task].condition == COND_WAIT_TIMEOUT)
            {
                tasks[current_task].timer--;

                if (tasks[current_task].timer == 0)
                {
                    tasks[current_task].condition = COND_READY;
                    tasks[current_task].event = TE_TIMEOUT;
                }
            }

            // If task is ready, run its task function
            if (tasks[current_task].condition == COND_READY)
            {
                tasks[current_task].task_function(current_task, tasks[current_task].state, tasks[current_task].event, 0);
            }

        }
	}
	return;
}


INT8U rtcs_acquire_id(void)
{
	static INT8U next_task_id = 0;
	
	return next_task_id++;
}

extern void rtcs_start_task(INT8U task_name, void (*task_function)(INT8U task_id, INT8U state, INT8U event, INT8U data))
{
	// Get available id from task list and assign to this task (if there is room for more tasks)
	INT8U task_id = rtcs_acquire_id();
	
	// Initialize the TCB and set condition to ready
	if (task_id < NUM_TASKS)
	{
		tasks[task_id].condition = COND_READY;
		tasks[task_id].name = task_name;
		tasks[task_id].state = 0; // State 0 = first execution
		tasks[task_id].event = 0;
		tasks[task_id].sem = 0; // First NUM_QUEUES semaphores are reserved for queues
		tasks[task_id].timer = 0;
		tasks[task_id].task_function = task_function;
	}
	
	return;
}

void rtcs_set_timeout(INT8U timeout)
{
	tasks[current_task].timer = timeout;
	tasks[current_task].condition = COND_WAIT_TIMEOUT;
}

void rtcs_set_state(INT8U state)
{
    tasks[current_task].state = state;

    return;
}

void rtcs_set_semaphore(INT8U semaphore)
{
	tasks[current_task].sem = semaphore;
	
	return;
}

void rtcs_wait_semaphore(INT8U semaphore)
{
    tasks[current_task].condition = COND_WAIT_SEMAPHORE;
		tasks[current_task].sem = semaphore;
    return;
}

TCB_T* rtcs_get_tasklist(void)
{
    static TCB_T tasklist_copy[NUM_TASKS];


    // Copy original tasklist
    INT8U i = 0;
    while (tasks[i].condition != COND_NO_TASK)
    {
        tasklist_copy[i].condition = tasks[i].condition;
        tasklist_copy[i].name = tasks[i].name;
        tasklist_copy[i].state = tasks[i].state;
        tasklist_copy[i].sem = tasks[i].sem;
        tasklist_copy[i].timer = tasks[i].timer;
        tasklist_copy[i].task_function = tasks[i].task_function;

        i++;
    }

    return tasklist_copy;
}
