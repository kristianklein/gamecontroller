#include "queue.h"
#include "semaphore.h"

typedef struct
{
	INT8U head;
	INT8U tail;
	INT8U capacity;
	INT8U semaphore;
	INT16U elems[MAX_ELEMS];
	INT8U size;
} QCB_T;

QCB_T queues[MAX_QUEUES];

BOOLEAN queue_open(INT8U queue_number, INT8U size)
{
	BOOLEAN queue_opened = 0;
	
	if (queue_number < MAX_QUEUES)
	{
		queues[queue_number].head = 0;
		queues[queue_number].tail = 0;
		queues[queue_number].capacity = 0;
		queues[queue_number].semaphore = queue_number;
        // The first MAX_QUEUES elements in semaphores[] MUST be reserved for queues!!
		
		if (size > MAX_ELEMS)
		{
		    queues[queue_number].size = MAX_ELEMS;
		}
		else
		{
		    queues[queue_number].size = size;
		}
		// Initialize the semaphore
		sem_set_value(queues[queue_number].semaphore, 1);
		
		queue_opened = 1;
	}
	
	return queue_opened;
}

BOOLEAN queue_enqueue(INT8U queue_number, INT16U value)
{
	BOOLEAN enqueue_success = 0;
	
	// Make sure queue is not full
	if (!queue_is_full(queue_number))
	{
		// Insert value at tail
		INT8U tail = queues[queue_number].tail;
		queues[queue_number].elems[tail] = value;

		// Increment tail (if tail > MAX_ELEMS -1, set tail = 0)
		tail = queue_increment(queue_number, tail);
		queues[queue_number].tail = tail;
		
		// Increment capacity
		queues[queue_number].capacity++;
		
		enqueue_success = 1;
	}
	
	return enqueue_success;
}

BOOLEAN queue_enqueue_force(INT8U queue_number, INT16U value)
{
    BOOLEAN enqueue_success = 0;

        // If queue is not full, do a regular enqueue
        if (!queue_is_full(queue_number))
        {
            queue_enqueue(queue_number, value);
        }
        else // If queue IS full, force enqueue and overwrite oldest value in queue
        {
            // Insert value at tail
            INT8U tail = queues[queue_number].tail;
            queues[queue_number].elems[tail] = value;

            // Increment tail (if tail > MAX_ELEMS -1, set tail = 0)
            tail = queue_increment(queue_number, tail);
            queues[queue_number].tail = tail;

            // Increment head, so queue is same size
            INT8U head = queues[queue_number].head;
            head = queue_increment(queue_number, head);
            queues[queue_number].head = head;

            // Note: capacity is unchanged!

            enqueue_success = 1;
        }

        return enqueue_success;
}

INT16U queue_dequeue(INT8U queue_number)
{
	INT16U result = 0;
	INT8U head = queues[queue_number].head;
	
	// Make sure queue is not empty
	if (!queue_is_empty(queue_number))
	{			
		// Take value from head as return value	
		result = queues[queue_number].elems[head];
		
		// Increment head 
		head = queue_increment(queue_number, head);
		queues[queue_number].head = head;
		
		// Decrement capacity
		queues[queue_number].capacity--;
	}
	
	return result;
}

INT16U queue_peek(INT8U queue_number)
{
	INT16U result = 0;
	
	if (!queue_is_empty(queue_number))
	{
		INT8U head = queues[queue_number].head;
		result = queues[queue_number].elems[head];
	}
	
	return result;
}

INT8U queue_capacity(INT8U queue_number)
{
	return queues[queue_number].capacity;
}

BOOLEAN queue_is_empty(INT8U queue_number)
{
	BOOLEAN is_empty = 0;
	
	if (queue_capacity(queue_number) == 0)
	{
		is_empty = 1;
	}
	
	return is_empty;
}

BOOLEAN queue_is_full(INT8U queue_number)
{
	BOOLEAN is_full = 0;
	
	if (queue_capacity(queue_number) == queues[queue_number].size)
	{
		is_full = 1;
	}
	
	return is_full;
}


INT8U queue_increment(INT8U queue_number, INT8U head_or_tail)
{
	INT8U result = 0;
	
	if (head_or_tail >= queues[queue_number].size - 1) // If at end of queue, wrap around to the beginning
	{
		result = 0;
	}
	else
	{
		result = head_or_tail + 1;
	}
	
	return result;
}

INT8U queue_sem(INT8U queue_number)
{
	return queues[queue_number].semaphore;
}


INT8U queue_tail(INT8U queue_number)
{
	INT8U result = 0;
	
	if (queue_number < MAX_QUEUES)
	{
		result = queues[queue_number].tail;
	}
	
	return result;
}
INT8U queue_head(INT8U queue_number)
{
	INT8U result = 0;
	
	if (queue_number < MAX_QUEUES)
	{
		result = queues[queue_number].head;
	}
	
	return result;
}
