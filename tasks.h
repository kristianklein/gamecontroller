#ifndef TASKS_H_
#define TASKS_H_

#include <stdint.h>
#include "emp_type.h"

enum TASKS {
    TASK_SYSBLINK,
    TASK_ADC,
    TASK_AMP,
    TASK_COORDS,
    TASK_GYRO,
    TASK_COMPFILT,
    TASK_BUTTON1,
    TASK_BUTTON2,
    TASK_UART_RX,
    TASK_CONTROL,
    TASK_UART_TX
};

void adc_task(INT8U id, INT8U state, INT8U event, INT8U data);
void amp_task(INT8U id, INT8U state, INT8U event, INT8U data);
void coords_task(INT8U id, INT8U state, INT8U event, INT8U data);
void gyro_task(INT8U id, INT8U state, INT8U event, INT8U data);
void compfilt_task(INT8U id, INT8U state, INT8U event, INT8U data);
void button1_task(INT8U id, INT8U state, INT8U event, INT8U data);
void button2_task(INT8U id, INT8U state, INT8U event, INT8U data);
void uart_rx_task(INT8U id, INT8U state, INT8U event, INT8U data);
void control_task(INT8U id, INT8U state, INT8U event, INT8U data);
void uart_tx_task(INT8U id, INT8U state, INT8U event, INT8U data);

#endif
