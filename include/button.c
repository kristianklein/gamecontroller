#include "button.h"
#include "tm4c123gh6pm.h"
#include "taskmodel.h"

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
