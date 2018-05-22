#include "sysblink.h"
#include "tm4c123gh6pm.h"
#include "systick.h"
#include "rtcs.h"

#include "queue.h"
#include "taskmodel.h" // DEBUGGING

#define STATUS_LED 0b01000000

void sysblink_init(void)
{
	// Enable clock for PORTD
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R3;
	
	// Set status LED as output
	GPIO_PORTD_DIR_R |= STATUS_LED;
	GPIO_PORTD_DEN_R |= STATUS_LED;
	GPIO_PORTD_PUR_R &= ~(STATUS_LED);
	
	return;
}

void sysblink_toggle_led(void)
{
	GPIO_PORTD_DATA_R ^= 0b01000000;
	
	return;
}

void sysblink_task(INT8U task_id, INT8U state, INT8U event, INT8U data)
{
    switch (state)
    {
        case 0: // INITIALIZED
            rtcs_set_timeout(MILLISEC(500));
            rtcs_set_state(1);
            break;
        case 1:
            sysblink_toggle_led();
            rtcs_set_state(0);
            break;
    }

    return;
}
