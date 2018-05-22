#include "button.h"
#include "tm4c123gh6pm.h"
#include "swtimer.h"
#include "messages.h"
#include "taskmodel.h"

#define SW1_TIMEOUT_BUFFER	SEB_SW1_TIMEOUT
#define SW1_EVENT_BUFFER		SEB_SW1_EVENT
#define SW2_TIMEOUT_BUFFER	SEB_SW2_TIMEOUT
#define SW2_EVENT_BUFFER		SEB_SW2_EVENT


// Button state type definition
typedef enum {INACTIVE, CLICKED, DOUBLECLICKED, RELEASED, HELD} BUTTON_STATE_T;

/*
void button_task(void)
{
	// Event buffer definitions
	static const INT8U event_buffers[] = {SEB_SW1_EVENT, SEB_SW2_EVENT};
	static const INT8U timeout_buffers[] = {SEB_SW1_TIMEOUT, SEB_SW2_TIMEOUT};
	static const INT8U timers[] = {SWT_SW1, SWT_SW2};
	
	// Static state variable
	static BUTTON_STATE_T button_state[] = {INACTIVE, INACTIVE};
	
	// Get timeout events
	BOOLEAN timeout[] = {get_event_msg(SEB_SW1_TIMEOUT), get_event_msg(SEB_SW2_TIMEOUT)};
	
	// Check all buttons for new events
	for (INT8U i = 0; i < NUM_BUTTONS; i++)
	{
		INT8U button_num = i + 1; // Index and button number is not the same
		
		switch (button_state[i])
		{
			case INACTIVE:
				if (button_active(button_num)) // Button has been clicked
				{
					put_event_msg(event_buffers[i], BE_CLICKED);
					start_sw_timer(timers[i], timeout_buffers[i], MILLISEC(HOLD_TIME));
					button_state[i] = CLICKED;
				}
				break;
			case DOUBLECLICKED:
				// No break!
			case CLICKED:
				if (button_active(button_num)) // Button held down
				{
					if (timeout[i])
					{
						put_event_msg(event_buffers[i], BE_HELD);
						button_state[i] = HELD;
					}
				}
				else // Button released
				{
					put_event_msg(event_buffers[i], BE_RELEASED);
					start_sw_timer(timers[i], timeout_buffers[i], MILLISEC(DOUBLECLICK_TIME));
					button_state[i] = RELEASED;
				}
				break;
			case RELEASED:
				if (timeout[i])
				{
					button_state[i] = INACTIVE;
				}
				else if (button_active(button_num))
				{
					put_event_msg(event_buffers[i], BE_DOUBLECLICKED);
					start_sw_timer(timers[i], timeout_buffers[i], MILLISEC(HOLD_TIME));
					button_state[i] = DOUBLECLICKED;
				}

				break;
			case HELD:
				if (!button_active(button_num))
				{
					put_event_msg(event_buffers[i], BE_RELEASED);
					button_state[i] = RELEASED;
				}
				break;
		}
	}
}
*/
BOOLEAN button_active(INT8U button_number)
{
	BOOLEAN active = 0;
	
	switch (button_number)
	{
		case SW1:
			if ( !(GPIO_PORTF_DATA_R & 0b00010000) )
			{
				active = 1;
			}
			break;
		case SW2:
			if ( !(GPIO_PORTF_DATA_R & 0b00000001) )
			{
				active = 1;
			}
			break;
		default:
			break;
	}

	return active;
}
