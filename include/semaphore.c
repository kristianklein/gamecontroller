#include "semaphore.h"

typedef struct
{
	INT8U condition; // Not really needed?
	INT8U type; // Not really needed?
	INT8U count;
} SCB_T;

SCB_T semaphores[NUM_SEMAPHORES];

BOOLEAN sem_wait(INT8U sem_number)
{
	BOOLEAN result = 0;
	
	if (sem_number < NUM_SEMAPHORES) // Check if exists
	{
		if (semaphores[sem_number].count) // If semaphore is available, take it
		{
		semaphores[sem_number].count--;
		result = 1;
		}
	}
	
	return result;
}

void sem_signal(INT8U sem_number)
{
	if (sem_number < NUM_SEMAPHORES) // Check if exists
	{
		semaphores[sem_number].count++;
	}
	
	return;
}

void sem_set_value(INT8U sem_number, INT8U value)
{
	if (sem_number < NUM_SEMAPHORES)
	{
		semaphores[sem_number].count = value;
	}
	
	return;
}