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

/// Ved ikke hovr jeg skal gøre af dette
#define NUMBER_OF_SAMPLES   43 // 43
#define NUMBER_OF_ADCS      4 // <--- BEMÆRK
#define AD0                 0
#define AD1                 1
#define AD2                 2
#define AD8                 8


//#define COEFF               1.9510
//#define LSB                 0.0129


INT8U lookup_array[NUMBER_OF_ADCS] = {Q_IR1,Q_IR2,Q_IR3,Q_IR4};     	//A look up array used to determine the Q.
INT8S test3;

void adc_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    launchpad_rgb(1,0,0);

    //ADC0_multiplex(8);
    //gfprintf(UART0, "Raw ADC: %d\r\n", ADC0_read());

    INT8U LOOK_UP_ADCS[NUMBER_OF_ADCS] = {AD0, AD1, AD2, AD8}; //AD0,AD1,AD2,
    INT16U ADC_array[NUMBER_OF_SAMPLES];                                 //A temporary array for ADC-data
    for(INT8U j = 0; j < NUMBER_OF_ADCS; j++)
    {
        ADC0_multiplex(LOOK_UP_ADCS[j]);                                //Change the input pin for the ADC.
        for(INT8U i = 0; i < NUMBER_OF_SAMPLES; i++)                    //This loop reads all the ADC-values and save them in an array
        {
            ADC_array[i] = ADC0_read();                                 //Save the ADC-values in an array
        }

        for(INT8U i = 0; i < NUMBER_OF_SAMPLES; i++)
        {
            ADC_array[i] = ADC_array[i] >> 4;                           //Shift all values right
            queue_enqueue(lookup_array[j], ADC_array[i]);               //Find the amplitude and save it in the amplitude Q
        }
    }


    rtcs_set_timeout(MILLISEC(15));                                     //Tell the scheduler to run this function again in 15ms
    launchpad_rgb(0,0,0);
    return;
}

void fft_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    INT8U adc_data[NUMBER_OF_SAMPLES];

    for(INT8U j = 0; j < NUMBER_OF_ADCS; j++)
    {
        for(INT8U i = 0; i < NUMBER_OF_SAMPLES; i++)
        {
            adc_data[i] = queue_dequeue(lookup_array[j]);
        }
        put_state_msg(j+2, uamplitude(adc_data, NUMBER_OF_SAMPLES));
        //gfprintf(UART0, "%d: %d\r\n", j+2, uamplitude(adc_data, NUMBER_OF_SAMPLES));
    }

    rtcs_set_timeout(MILLISEC(15));                                     //Tell the scheduler to run this function again in 15ms


    /*
     FP32 Q0,Q1,Q2;
    FP32 mag;
    INT16U amp;

    for(INT8U j = 0; j < NUMBER_OF_ADCS; j++)                       //For all input Q's.
    {
        Q0 = 0;                                                     //Init the temporary calc. variables
        Q1 = 0;
        Q2 = 0;

        for(INT8U i = 0; i > NUMBER_OF_SAMPLES; i++)                //For all ADC'values
        {
            Q0 = COEFF*Q1-Q2+(queue_dequeue(lookup_array[j])*LSB);
            Q2 = Q1;
            Q1 = Q0;
        }
        mag = sqrt((Q1*Q1+Q2*Q2-Q1*Q2*COEFF))*2/NUMBER_OF_SAMPLES;  //Compute the abselute amplitude
        amp = mag*NUMBER_OF_DECIMALS;                               //Multiply the result with 100 to save decimals when sending to Q
        queue_enqueue(Q_IR_AMP, amp);                               //Save the values in the Q
	}
    rtcs_set_timeout(MILLISEC(15));

     */
    return;
}

void coords_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    INT16U ir1_amp, ir2_amp, ir3_amp, ir4_amp;
    INT8S x_coord, y_coord;

    // Get amplitudes from IR state buffers
    ir1_amp = get_state_msg(SSB_IR1_AMP); // Bottom in array (ADC3)
    ir2_amp = get_state_msg(SSB_IR2_AMP); // Right in array (ADC2)
    ir3_amp = get_state_msg(SSB_IR3_AMP); // Left in array (ADC1)
    ir4_amp = get_state_msg(SSB_IR4_AMP); // Up in array (ADC9)
    /*
        queue_enqueue(Q_UART_TX, ir1_amp);
        queue_enqueue(Q_UART_TX, ir2_amp);
        queue_enqueue(Q_UART_TX, ir3_amp);
        queue_enqueue(Q_UART_TX, ir4_amp);
    */

    //gfprintf(UART0, "IR1: %d, IR2: %d, IR3: %d, IR4: %d \r\n", ir1_amp, ir2_amp, ir3_amp, ir4_amp);

    // Calculate x and y coordinates
    x_coord = ir_calc_x(ir3_amp, ir2_amp);
    y_coord = ir_calc_y(ir1_amp, ir4_amp);

    // Put coordinates in coordinate state buffer
    put_state_msg(SSB_COORDS_X, x_coord);
    put_state_msg(SSB_COORDS_Y, y_coord);

    //queue_enqueue(Q_UART_TX,x_coord);
    //queue_enqueue(Q_UART_TX,y_coord);
   return;
}

void gyro_task(INT8U my_id, INT8U my_state, INT8U event, INT8U data){
    INT16S accelerometer[3], gyro[3];
    data_read(accelerometer, gyro);

    for(int i = 0; i < 3; i++) {
        queue_enqueue(Q_GYRO_RAW, accelerometer[i]);
        queue_enqueue(Q_GYRO_RAW, gyro[i]);
    }
    return;
}

void lpf_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    static float roll, pitch;

    INT16S accelerometer[3], gyro[3];
    INT8S roll_8bit;
    for(int i = 0; i < 3; i++) {
        accelerometer[i] = queue_dequeue(Q_GYRO_RAW);
        gyro[i] = queue_dequeue(Q_GYRO_RAW);

    }
    complementary_filter(accelerometer, gyro, &pitch, &roll);

    roll_8bit = map_data(roll-get_roll_cali());
    put_state_msg(SSB_GYRO, roll_8bit);
    return;
}

void button1_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    switch (state)
    {
        case 0: // Inactive - check if button has been pressed
            if (button_active(1))
            {
                rtcs_set_state(1);
                queue_enqueue(Q_BUTTON, BE_SW1_CLICKED);
            }
            break;
        case 1: // Active - check if button has been released
            if (!button_active(1))
            {
                rtcs_set_state(0);
                queue_enqueue(Q_BUTTON, BE_SW1_RELEASED);
            }
            break;
    }
}

void button2_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    switch (state)
    {
        case 0: // Inactive - check if button has been pressed
            if (button_active(2))
            {
                rtcs_set_state(1);
                queue_enqueue(Q_BUTTON, BE_SW2_CLICKED);
            }
            break;
        case 1: // Active - check if button has been released
            if (!button_active(2))
            {
                rtcs_set_state(0);
                queue_enqueue(Q_BUTTON, BE_SW2_RELEASED);
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
    /*
    if (uart1_rx_rdy()) // If a byte is received, put it in the RX buffer
    {
        queue_enqueue(Q_UART1_RX, uart1_getc());
    }
     */
    return;
}

void control_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    static INT8U button = 0;

    // Get coordinates and roll from state buffers
    INT8S x_coord = get_state_msg(SSB_COORDS_X);
    INT8S y_coord = get_state_msg(SSB_COORDS_Y);
    INT8S z_roll = get_state_msg(SSB_GYRO);


    // Get button event is available
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

    //gfprintf(UART0, "x-coord: %d, y-coord: %d, z-roll: %d, button: %d", x_coord, y_coord, z_roll, button);
    hid_send_controls(x_coord, y_coord, z_roll, button, 0);

    rtcs_set_timeout(MILLISEC(15));
    return;
}

void uart_tx_task(INT8U id, INT8U state, INT8U event, INT8U data)
{
    /*while (!queue_is_empty(Q_UART_TX)) // Send entire queue
    {
        if (uart0_tx_rdy())
        {
            uart0_putc(queue_dequeue(Q_UART_TX));
        }
    }*/

    while (!queue_is_empty(Q_UART1_TX)) // Send entire queue
    {
        if (uart1_tx_rdy())
        {
            INT8U transmit_value = queue_dequeue(Q_UART1_TX);
            uart1_putc(transmit_value);
        }
    }
    return;
}
