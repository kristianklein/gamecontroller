#include "hid.h"
#include "queue.h"
#include "string.h"
#include "systick.h"
#include "taskmodel.h"
#include "file.h"
#include "tasks.h"
#include "uart1.h"

void hid_init()
{
	us_delay(50);
	gfprintf(UART1, "$$$");						//Go into CMD mode
	us_delay(50);
	gfprintf(UART1, "SN,HIDGamepad\r\n");		//Set device name to HIDGamepad
	us_delay(50);
	gfprintf(UART1, "S~,6\r\n");				//Enable HID profile
	//gfprintf(UART1, "S~,0\r\n");
	us_delay(600);
	gfprintf(UART1, "SH,0210\r\n");				//Ser device as 0240 = joystick (0200=keyboard, 0210=gamepad, 0220=mouse, 0230=CUMBO, 0240=joystick 0250=Reserved)
	us_delay(200);
	gfprintf(UART1, "R,1\r\n");					//Reboot to use HID profile
	us_delay(400);
}

void hid_send_controls(INT8S x, INT8S y, INT8S z, INT8U buttonLSB, INT8U buttonMSB)
{
    queue_enqueue(Q_UART1_TX, 0xFD);
    queue_enqueue(Q_UART1_TX, 0x06);
    queue_enqueue(Q_UART1_TX, x);
    queue_enqueue(Q_UART1_TX, y);
    queue_enqueue(Q_UART1_TX, z);
    queue_enqueue(Q_UART1_TX, 0x40);
    queue_enqueue(Q_UART1_TX, buttonLSB);
    queue_enqueue(Q_UART1_TX, 0);
    return;
}

void hid_send_controls0(INT8S x, INT8S y, INT8S z, INT8U buttonLSB, INT8U buttonMSB)
{

    queue_enqueue(Q_UART_TX, 0xFD);
    queue_enqueue(Q_UART_TX, 0x06);
    queue_enqueue(Q_UART_TX, 0x00);
    queue_enqueue(Q_UART_TX, 0x00);
    queue_enqueue(Q_UART_TX, 0x40);
    queue_enqueue(Q_UART_TX, 0x41);
    queue_enqueue(Q_UART_TX, 0x42);
    queue_enqueue(Q_UART_TX, 0x00);

    return;
}
