#include "tasks.h"
#include "taskmodel.h"
#include "queue.h"
#include "uart0.h"
#include "file.h"
#include "string.h"
#include "ADC0.h"
#include "global_def.h"
#include "systick.h"
#include "rtcs.h"
#include "amplitude.h"
#include "messages.h"
#include "ir_sensor.h"
#include "uart1.h"
#include "hid.h"
#include "gyro.h"
#include "launchpad.h"
#include "button.h"
#include "swtimer.h"
#include "semaphore.h"

// Defines for ADC task
#define NUMBER_OF_SAMPLES   27
#define NUMBER_OF_ADCS      4
#define AD0                 0
#define AD1                 1
#define AD2                 2
#define AD8                 8

const INT8U lookup_array[NUMBER_OF_ADCS] = {Q_IR1,Q_IR2,Q_IR3,Q_IR4}; // A look up array used to determine which queue to use.

void adc_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    INT8U LOOK_UP_ADCS[NUMBER_OF_ADCS] = {AD0, AD1, AD2, AD8}; // AD0,AD1,AD2,
    INT16U ADC_array[NUMBER_OF_SAMPLES];                                 // A temporary array for ADC-data
    for(INT8U j = 0; j < NUMBER_OF_ADCS; j++)
    {
        ADC0_multiplex(LOOK_UP_ADCS[j]);                                // Change the input pin for the ADC.
        for(INT8U i = 0; i < NUMBER_OF_SAMPLES; i++)                    // This loop reads all the ADC-values and save them in an array
        {
            ADC_array[i] = ADC0_read();                                 // Save the ADC-values in an array
        }

        for(INT8U i = 0; i < NUMBER_OF_SAMPLES; i++)
        {
            ADC_array[i] = ADC_array[i] >> 4;                           // Shift all values right
            queue_enqueue(lookup_array[j], ADC_array[i]);               // Find the amplitude and save it in the amplitude Q
        }
    }

    // Signal SEM_DATA_READY semaphore
    sem_signal(SEM_DATA_READY);

    // Only run task every 15 ms (66.7 Hz)
    rtcs_set_timeout(MILLISEC(15));                                     // Tell the scheduler to run this function again in 15ms
    return;
}

void amp_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    INT8U adc_data[NUMBER_OF_SAMPLES];

    switch (state)
    {
        case 0:
            // Wait for semaphore
            rtcs_wait_semaphore(SEM_DATA_READY);
            rtcs_set_state(1);
            break;
        case 1:
            for(INT8U j = 0; j < NUMBER_OF_ADCS; j++)
            {
                for(INT8U i = 0; i < NUMBER_OF_SAMPLES; i++)
                {
                    adc_data[i] = queue_dequeue(lookup_array[j]);
                }
                put_state_msg(j+2, uamplitude(adc_data, NUMBER_OF_SAMPLES));
            }

            // Set SEM_AMP_UPDATED semaphore
            sem_signal(SEM_AMP_UPDATED);

            // Wait for SEM_DATA_READY semaphore
            rtcs_wait_semaphore(SEM_DATA_READY);
            break;
    }

    return;
}

void coords_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    INT16U ir1_amp, ir2_amp, ir3_amp, ir4_amp;
    INT8S x_coord, y_coord;

    switch (state)
    {
        case 0:
            // Initialize task: Wait for SEM_AMP_UPDATED
            rtcs_wait_semaphore(SEM_AMP_UPDATED);
            rtcs_set_state(1);
            break;
        case 1:
            // Get amplitudes from IR state buffers
            ir1_amp = get_state_msg(SSB_IR1_AMP); // Bottom in array (ADC3)
            ir2_amp = get_state_msg(SSB_IR2_AMP); // Right in array (ADC2)
            ir3_amp = get_state_msg(SSB_IR3_AMP); // Left in array (ADC1)
            ir4_amp = get_state_msg(SSB_IR4_AMP); // Up in array (ADC9)

            // Calculate x and y coordinates
            x_coord = ir_calc_x(ir3_amp, ir2_amp);
            y_coord = ir_calc_y(ir1_amp, ir4_amp);

            // Put coordinates in coordinate state buffer
            put_state_msg(SSB_COORDS_X, x_coord);
            put_state_msg(SSB_COORDS_Y, y_coord);

            // Signal SEM_NEW_EVENT semaphore
            sem_wait(SEM_NEW_EVENT); // Take semaphore first, in case another task already signalled it.
            sem_signal(SEM_NEW_EVENT);

            // Wait for semaphore again.
            rtcs_wait_semaphore(SEM_AMP_UPDATED);
            break;
    }

    return;
}

void gyro_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data){
    INT16S accelerometer[3], gyro[3];
    data_read(accelerometer, gyro);

    for(int i = 0; i < 3; i++) {
        queue_enqueue(Q_GYRO_RAW, accelerometer[i]);
        queue_enqueue(Q_GYRO_RAW, gyro[i]);
    }

    // Set GYRO_READY semaphore
    sem_signal(SEM_GYRO_READY);

    // Only run every 15 ms
    rtcs_set_timeout(MILLISEC(15));
    return;
}

void compfilt_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    static float roll, pitch;

    INT16S accelerometer[3], gyro[3];
    INT8S roll_8bit;

    switch (state)
    {
        case 0: // Initialization of task
            rtcs_wait_semaphore(SEM_GYRO_READY);
            rtcs_set_state(1);
            break;
        case 1:
            for(int i = 0; i < 3; i++) {
                accelerometer[i] = queue_dequeue(Q_GYRO_RAW);
                gyro[i] = queue_dequeue(Q_GYRO_RAW);
            }

            complementary_filter(accelerometer, gyro, &pitch, &roll);

            roll_8bit = map_data(roll-get_roll_cali());
            put_state_msg(SSB_GYRO, roll_8bit);

            // Signal SEM_NEW_EVENT semaphore
            sem_wait(SEM_NEW_EVENT); // Take semaphore first, in case another task already signalled it.
            sem_signal(SEM_NEW_EVENT);

            // Wait for GYRO_READY semaphore
            rtcs_wait_semaphore(SEM_GYRO_READY);
            break;
    }

    return;
}

void button1_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    switch (state)
    {
        case 0: // Inactive - check if button has been pressed
            if (button_active(SW1))
            {
                rtcs_set_state(1);
                queue_enqueue(Q_BUTTON, BE_SW1_CLICKED);

                // Signal SEM_NEW_EVENT semaphore
                sem_wait(SEM_NEW_EVENT); // Take semaphore first, in case another task already signalled it.
                sem_signal(SEM_NEW_EVENT);
            }
            break;
        case 1: // Active - check if button has been released
            if (!button_active(SW1))
            {
                rtcs_set_state(0);
                queue_enqueue(Q_BUTTON, BE_SW1_RELEASED);

                // Signal SEM_NEW_EVENT semaphore
                sem_wait(SEM_NEW_EVENT); // Take semaphore first, in case another task already signalled it.
                sem_signal(SEM_NEW_EVENT);
            }
            break;
    }
    return;
}

void button2_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    switch (state)
    {
        case 0: // Inactive - check if button has been pressed
            if (button_active(SW2))
            {
                rtcs_set_state(1);
                queue_enqueue(Q_BUTTON, BE_SW2_CLICKED);

                // Signal SEM_NEW_EVENT semaphore
                sem_wait(SEM_NEW_EVENT); // Take semaphore first, in case another task already signalled it.
                sem_signal(SEM_NEW_EVENT);
            }
            break;
        case 1: // Active - check if button has been released
            if (!button_active(SW2))
            {
                rtcs_set_state(0);
                queue_enqueue(Q_BUTTON, BE_SW2_RELEASED);

                // Signal SEM_NEW_EVENT semaphore
                sem_wait(SEM_NEW_EVENT); // Take semaphore first, in case another task already signalled it.
                sem_signal(SEM_NEW_EVENT);
            }
            break;
    }
}

void uart_rx_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    if (uart0_rx_rdy()) // If a byte is received, put it in the RX buffer
    {
        queue_enqueue(Q_UART_RX, uart0_getc());
    }
    return;
}

void control_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    static INT8U button = 0;
    INT8S x_coord, y_coord, z_roll;

    switch (state)
    {
        case 0:
            // Wait for SEM_NEW_EVENT semaphore
            rtcs_wait_semaphore(SEM_NEW_EVENT);
            rtcs_set_state(1);
            break;
        case 1:
            // Get button event if available
            if (!queue_is_empty(Q_BUTTON))
            {
                INT8U button_event = queue_dequeue(Q_BUTTON);

                switch (button_event)
                {
                    case BE_SW1_CLICKED:
                        button |= 0b00000001;
                        break;
                    case BE_SW1_RELEASED:
                        button &= ~(0b00000001);
                        break;
                    case BE_SW2_CLICKED:
                        button |= 0b00000010;
                        break;
                    case BE_SW2_RELEASED:
                        button &= ~(0b00000010);
                        break;
                    default:
                        break;
                }
            }

            // Get coordinates and roll from state buffers
            x_coord = get_state_msg(SSB_COORDS_X);
            y_coord = get_state_msg(SSB_COORDS_Y);
            z_roll = get_state_msg(SSB_GYRO);

            hid_send_controls(x_coord, y_coord, z_roll, button, 0);

            // Wait for semaphore
            rtcs_wait_semaphore(SEM_NEW_EVENT);

            break;
    }

    return;
}

void uart_tx_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    launchpad_rgb(1,0,0);
    while (!queue_is_empty(Q_UART_TX)) // Send entire queue
    {
        if (uart0_tx_rdy())
        {
            uart0_putc(queue_dequeue(Q_UART_TX));
        }
    }

    while (!queue_is_empty(Q_UART1_TX)) // Send entire queue
    {
        if (uart1_tx_rdy())
        {
            INT8U transmit_value = queue_dequeue(Q_UART1_TX);
            uart1_putc(transmit_value);
        }
    }
    launchpad_rgb(0,0,0);
    return;
}
