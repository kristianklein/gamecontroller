#ifndef FILE_GET_PUT_H
#define FILE_GET_PUT_H

#include <stdint.h>
#include "emp_type.h"

BOOLEAN uart0_put(INT8U character);
BOOLEAN uart0_get(INT8U* character);
BOOLEAN uart0_peek(INT8U* character);
BOOLEAN uart1_put(INT8U character);
BOOLEAN uart1_get(INT8U* character);
BOOLEAN uart1_peek(INT8U* character);
BOOLEAN button_get(INT8U* character);
BOOLEAN button_peek(INT8U* character);

#endif
