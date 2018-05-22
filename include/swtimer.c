#include "swtimer.h"
#include "messages.h"
#include "taskmodel.h"

typedef struct SWTIMER_T {
	INT16U value;
	INT8U event_buffer;
} SWTIMER_T;

SWTIMER_T timers[NUM_TIMERS];

void sw_timer_init(void)
{
	// Initialize all timers to 0
	for (INT8U i = 0; i < NUM_TIMERS; i++)
	{
		timers[i].value = 0;
		timers[i].event_buffer = 0;
	}
	
	return;
}

void sw_timer_update(void)
{
	// Check if any of the active timers have timed out
	
	for (INT8U i = 0; i < NUM_TIMERS; i++)
	{
		if (sw_timer_in_use(i))
		{
			timers[i].value--;
			
			if (timers[i].value == 0)
			{
				// Put message in event buffer when the timeout event occurs
				put_event_msg(timers[i].event_buffer, TE_TIMEOUT);
			}
		}
	}
	
	return;
}

void start_sw_timer(INT8U timer_num, INT8U event_buffer, INT16U time)
{
	if (timer_num < NUM_TIMERS) // If a timer is available
	{
		timers[timer_num].value = time;
		timers[timer_num].event_buffer = event_buffer;
	}
	
	return;
}

BOOLEAN sw_timer_in_use(INT8U timer_num)
{
	BOOLEAN in_use = 0;
	
	if (timers[timer_num].value != 0)
	{
		in_use = 1;
	}
	
	return in_use;
}