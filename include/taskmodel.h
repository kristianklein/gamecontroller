#include "queue.h"

///////////////////////////////
// Shared event buffer (SEB) //
///////////////////////////////
enum EVENT_BUFFERS 
{
	SEB_TRASH,
	SEB_SW1_EVENT,
	SEB_SW1_TIMEOUT,
	SEB_SW2_EVENT,
	SEB_SW2_TIMEOUT,
	SEB_SYSBLINK
};

/////////////////////////////////////////////////////////
// Shared state buffer / Shared state memory (SSB/SSM) //
/////////////////////////////////////////////////////////
enum STATE_BUFFERS 
{
	SSB_SW1,
	SSB_SW2,
	SSB_IR1_AMP,
	SSB_IR2_AMP,
	SSB_IR3_AMP,
	SSB_IR4_AMP,
	SSB_COORDS_X,
	SSB_COORDS_Y,
	SSB_GYRO
};

////////////
// Events //
////////////
enum EVENTS 
{
	// Generel events
	GE_NO_EVENT, // Must be the first event so it is 0
	
	// Launchpad button events
	BE_SW1_CLICKED,
	BE_SW1_DOUBLECLICKED,
	BE_SW1_HELD,
	BE_SW1_RELEASED,
	BE_SW2_CLICKED,
	BE_SW2_DOUBLECLICKED,
	BE_SW2_HELD,
	BE_SW2_RELEASED,
	
	// Timer events
	TE_TIMEOUT,
	
	// Semaphore events
	SE_SIGNAL,
	SE_WAIT,
};

/////////////////////
// Software timers //
/////////////////////
enum SWTIMERS
{
	SWT_SYSBLINK,
	SWT_SW1,
	SWT_SW2
};

////////////
// Queues //
////////////

enum QUEUES
{
	Q_BUTTON,
	Q_UART_RX,
	Q_UART_TX,
	Q_UART1_RX,
    Q_UART1_TX,
	Q_IR1,
	Q_IR2,
	Q_IR3,
	Q_IR4,
	Q_GYRO_RAW
};

////////////////
// SEMAPHORES //
////////////////

enum SEMAPHORES
{
	SEM_DATA_READY = MAX_QUEUES,	// Start here, since the first NUM_QUEUES elements are reserved for queues
	SEM_AMP_UPDATED,
	SEM_NEW_EVENT,
};
