#ifndef UART1_H
#define UART1_H

#include <stdint.h>
#include "emp_type.h"

BOOLEAN uart1_rx_rdy();
INT8U uart1_getc();
BOOLEAN uart1_tx_rdy();
void uart1_putc( INT8U character );
void uart1_init( INT32U baud_rate, INT8U databits, INT8U stopbits, INT8U parity );

#endif
