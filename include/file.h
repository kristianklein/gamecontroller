#ifndef FILE_H
#define FILE_H

#include <stdint.h>
#include "emp_type.h"

typedef INT8U* FILE;

#define UART0 		0
#define UART1       1
#define BUTTON		2

#define MAX_FILES 8

void file_init(void);
BOOLEAN file_put(FILE file, INT8U character);
BOOLEAN file_get(FILE file, INT8U* character);
BOOLEAN file_peek(FILE file, INT8U* character);


#endif
