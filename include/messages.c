#include "messages.h"

// Event and state buffer arrays
INT8U event_msg[NUM_BUFFERS];
INT8U state_msg[NUM_BUFFERS];

INT8U get_event_msg(INT8U event_num)
{
	INT8U message = 0;
	
	if (event_num < NUM_BUFFERS) // Input validation
	{
		message = event_msg[event_num];
		event_msg[event_num] = 0; // Clear event message on read
	}
	
	return message;
}

void put_event_msg(INT8U event_num, INT8U event)
{
	if (event_num < NUM_BUFFERS)
	{
		event_msg[event_num] = event;
	}
	
	return;
}

INT8U get_state_msg(INT8U state_num)
{
	INT8U state = 0;
	
	if (state_num < NUM_BUFFERS)
	{
		state = state_msg[state_num];
	}
	
	return state;
}

void put_state_msg(INT8U state_num, INT8U state)
{
	if (state_num < NUM_BUFFERS)
	{
		state_msg[state_num] = state;
	}
	
	return;
}

