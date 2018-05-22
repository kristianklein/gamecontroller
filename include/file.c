#include "file.h"
#include "global_def.h"
#include "file_func.h"

typedef struct
{
	BOOLEAN (*put)(INT8U);
	BOOLEAN (*get)(INT8U*);
	BOOLEAN (*peek)(INT8U*);
} FCB_T;

FCB_T files[MAX_FILES];

void file_init(void)
{
	for (INT8U i = 0; i < MAX_FILES; i++)
	{
		files[i].put = NULL;
		files[i].get = NULL;
		files[i].peek = NULL;
	}
	
	// Initialize files
	// Functions get/put character to/from respective queues
	files[UART0].put = uart0_put;
	files[UART0].get = uart0_get;
	files[UART0].peek = uart0_peek;
	files[UART1].put = uart1_put;
	files[UART1].get = uart1_get;
	files[UART1].peek = uart1_peek;
	files[BUTTON].get = button_get;
	files[BUTTON].peek = button_peek;
	
	return;
}

BOOLEAN file_put(FILE file, INT8U character)
{
	BOOLEAN result = 0;
	
	if (files[(INT8U)file].put) // If file has a put-function
	{
		result = files[(INT8U)file].put(character); // Put character in file
	}
	
	return result;
}

BOOLEAN file_get(FILE file, INT8U* character)
{
	BOOLEAN result = 0;
	
	if (files[(INT8U)file].get) // If file has a get-function
	{
		result = files[(INT8U)file].get(character); // Get a character and place it where the pointer is pointing
	}
	
	return result;
}

BOOLEAN file_peek(FILE file, INT8U* character)
{
	BOOLEAN result = 0;
	
	if (files[(INT8U)file].peek) // If file has a peek-function
	{
		result = files[(INT8U)file].peek(character); // Get a character and place it where the pointer is pointing
	}
	
	return result;
}
