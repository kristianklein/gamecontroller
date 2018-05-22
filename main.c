#include "uart0.h"
#include "launchpad.h"
#include "rtcs.h"
#include "tasks.h"
#include "taskmodel.h"
#include "ADC0.h"
#include "uart1.h"
#include "file.h"
#include "gyro.h"
#include "semaphore.h"

int main(void)
{
    // Open queues
    queue_open(Q_BUTTON, 128);
    queue_open(Q_UART_RX, 128);
    queue_open(Q_UART_TX, 128);
    queue_open(Q_IR1, 128);
    queue_open(Q_IR2, 128);
    queue_open(Q_IR3, 128);
    queue_open(Q_IR4, 128);
    queue_open(Q_GYRO_RAW, 128);
    queue_open(Q_UART1_RX, 128);
    queue_open(Q_UART1_TX, 128);

    // Initialization functions
    uart0_init(115200, 8, 1, 0);
    uart1_init(115200, 8, 1, 0);
	ADC0_init_seq3_ch1();
	launchpad_init();
    file_init();
    I2C_init();

    rtcs_init(); // Initializes systick, sysblink and sw_timer.

    // Initialize semaphores
    sem_set_value(SEM_DATA_READY, 0);
    sem_set_value(SEM_AMP_UPDATED, 0);
    sem_set_value(SEM_NEW_EVENT, 0);
    sem_set_value(SEM_GYRO_READY, 0);

    // Start tasks (sysblink is started automatically in rtcs_init())
    rtcs_start_task(TASK_ADC, adc_task);
    rtcs_start_task(TASK_AMP, amp_task);
    rtcs_start_task(TASK_COORDS, coords_task);
    rtcs_start_task(TASK_GYRO, gyro_task);
    rtcs_start_task(TASK_COMPFILT, compfilt_task);
    rtcs_start_task(TASK_BUTTON1, button1_task);
    rtcs_start_task(TASK_BUTTON2, button2_task);
    rtcs_start_task(TASK_UART_RX, uart_rx_task);
    rtcs_start_task(TASK_CONTROL, control_task);
    rtcs_start_task(TASK_UART_TX, uart_tx_task);

    // Start scheduler
    rtcs_schedule();

    return 0;
}
