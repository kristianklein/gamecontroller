#include "file_func.h"
#include "queue.h"
#include "taskmodel.h"

BOOLEAN uart0_put(INT8U character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_full(Q_UART_TX))
	{
		queue_enqueue(Q_UART_TX, character);
		
		result = 1;
	}		
	
	return result;
}


BOOLEAN uart0_get(INT8U* character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_empty(Q_UART_RX))
	{
		*character = queue_dequeue(Q_UART_RX);
		
		result = 1;
	}
	
	return result;
}

BOOLEAN uart0_peek(INT8U* character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_empty(Q_UART_RX))
	{
		*character = queue_peek(Q_UART_RX);
		
		result = 1;
	}
	
	return result;
}

BOOLEAN uart1_put(INT8U character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_full(Q_UART1_TX))
	{
		queue_enqueue(Q_UART1_TX, character);
		
		result = 1;
	}		
	
	return result;
}


BOOLEAN uart1_get(INT8U* character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_empty(Q_UART1_RX))
	{
		*character = queue_dequeue(Q_UART1_RX);
		
		result = 1;
	}
	
	return result;
}

BOOLEAN uart1_peek(INT8U* character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_empty(Q_UART1_RX))
	{
		*character = queue_peek(Q_UART1_RX);
		
		result = 1;
	}
	
	return result;
}

BOOLEAN button_get(INT8U* character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_empty(Q_BUTTON))
	{
		*character = queue_dequeue(Q_BUTTON);
	}
	
	return result;
}

BOOLEAN button_peek(INT8U* character)
{
	BOOLEAN result = 0;
	
	if (!queue_is_empty(Q_BUTTON))
	{
		*character = queue_peek(Q_BUTTON);
	}
	
	return result;
}
