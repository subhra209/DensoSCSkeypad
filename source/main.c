
/*
*------------------------------------------------------------------------------
* main.c
*
*------------------------------------------------------------------------------
*/


#pragma config FOSC 	= INTIO67
#pragma config PLLCFG 	= OFF
#pragma config PRICLKEN = OFF
#pragma config FCMEN 	= OFF
#pragma config IESO 	= OFF
#pragma config PWRTEN   = OFF
#pragma config BOREN 	= OFF
#pragma config BORV 	= 285
#pragma config WDTEN    = OFF
#pragma config WDTPS 	= 512
#pragma config CCP2MX 	= PORTC1
#pragma config PBADEN 	= OFF
#pragma config MCLRE 	= EXTMCLR
#pragma config STVREN 	= OFF
#pragma config XINST 	= OFF
#pragma config LVP      = OFF
#pragma config CP0      = OFF
#pragma config CP1      = OFF
#pragma config CP2      = OFF
#pragma config CP3      = OFF
#pragma config CPB      = OFF
#pragma config CPD      = OFF
#pragma config WRT0     = OFF
#pragma config WRT1     = OFF
#pragma config WRT2     = OFF
#pragma config WRT3     = OFF
#pragma config WRTB     = OFF       
#pragma config WRTC     = OFF
#pragma config WRTD     = OFF
#pragma config EBTR0    = OFF
#pragma config EBTR1    = OFF
#pragma config EBTR2    = OFF
#pragma config EBTR3    = OFF
#pragma config EBTRB    = OFF





/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <timers.h>				// Timer library functions
#include <delays.h>				// Delay library functions
#include <string.h>				// String library functions
#include "board.h"				// board setup
#include "timer.h"
#include "heartbeat.h"
#include "config.h"
#include "Keypad.h"
#include "lcd.h"
#include "string.h"
#include "ui.h"
#include "app.h"
#include "communication.h"
#include "uart.h"

/*
*------------------------------------------------------------------------------
* Private Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Processor config bits
*------------------------------------------------------------------------------
*/



/*
*------------------------------------------------------------------------------
* Private Macros
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Data Types
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Variables
*------------------------------------------------------------------------------
*/


/*

*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Function Prototypes (static)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* void main(void)

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/


void main(void)
{
	unsigned long temp;

	BRD_init();			//board initialization

	TMR0_init(tickPeriod,0);		//initialize timer0

	LCD_init();
	LinearKeyPad_init();
	COM_init(CMD_SOP,CMD_EOP,RESP_SOP,RESP_EOP,APP_comCallBack);

#ifdef TEST_LCD
	for(i = 0 ; i< 26; i++)
	{
		LCD_putChar(i+'A');
		DelayMs(200);
	}
	LCD_clear();
#endif


#ifdef TOWER_LAMP_TEST
	LAMP_GREEN = 1;
	DelayMs(1000);
	LAMP_GREEN = 0;

	LAMP_YELLOW = 1;
	DelayMs(1000);
	LAMP_YELLOW = 0;

	LAMP_RED = 1;
	DelayMs(1000);
	LAMP_RED = 0;


	BUZZER = 1;
	DelayMs(1000);
	BUZZER = 0;

#endif


	KEYPAD_init();
	


	UI_init();			//MUST BE DONE AFTER IAS INIT

	APP_init();
#ifdef __UART_TEST__
	COM_txStr("IDEONICS ANDON TERMINAL ");
#endif	
	EnableInterrupts();		//Interrupts initialization

	//Heart Beat to blink at every 500ms
	temp = (500UL *1000UL)/TIMER0_TIMEOUT_DURATION;
	
	while(1)
	{

		//Heart Beat to blink at every 500ms
	`	if(heartBeatCount >= temp )
		{			
			HB_task();
			heartBeatCount = 0;
		}

		if( keypadUpdateCount >= 8 )
		{
			KEYPAD_task();
			LinearKeyPad_scan();
			keypadUpdateCount = 0;
		}


		if( appUpdateCount >= 250 )
		{
			APP_task();
			appUpdateCount = 0;
		}
		//ClrWdt();
	


	#ifdef KEYPAD_TEST
			if( KEYPAD_read(&scancode, &duration) == TRUE )
				LCD_putChar(scancode);
	#else
		if( uiUpdateCount >=40 )
		{
			
			UI_task();
			uiUpdateCount = 0;
	
			
		}


	}
}

/*
*  End of main.c
*/
