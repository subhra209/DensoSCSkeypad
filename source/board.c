
/*
*------------------------------------------------------------------------------
* device.c
*
* Board specipic drivers module(BSP)
*
*
* The copyright notice above does not evidence any
* actual or intended publication of such source code.
*
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


#include "board.h"
#include "config.h"
#include "typedefs.h"




/*
*------------------------------------------------------------------------------
* Private Defines
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
UINT16 tickPeriod = 0;
/*
*------------------------------------------------------------------------------
* Private Variables (static)
*------------------------------------------------------------------------------
*/

static BOOL ledState;

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
* void InitializeBoard(void)

* Summary	: This function configures all i/o pin directions
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void BRD_init(void)
{
	unsigned long clock, temp;

	//Store sytem clock
	clock = SYSTEM_CLOCK;

	switch(clock)
	{
		case MHz_64:
		default:
				OSCCON = 0X70; 		//internal oscillator 64MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;

		case MHz_32:
				OSCCON = 0X60; 		//internal oscillator 32MHz
				OSCTUNEbits.PLLEN = 1;	//PLL Enable
		break;
		
		case MHz_16:
				OSCCON = 0X70; 		//internal oscillator 16MHz
				OSCTUNEbits.PLLEN = 0;	//PLL Enable
		break;
	}

	//calculating tick period for timer-0
	temp = clock >> 2;	
	temp *= (TIMER0_TIMEOUT_DURATION/1000);
	temp /= 1000;		

	tickPeriod = (FULLSCALE_16BIT - (UINT16)temp);



	// set all anolog channels as Digital I/O
	ADCON0 = 0x00;	
	ANSELA = 0;
	ANSELB = 0;
	ANSELC = 0;
	ANSELD = 0;
	ANSELE = 0;


	//Heart Beat
	HEART_BEAT_DIRECTION = PORT_OUT;

	//I2C Communication
	I2C_SCL_DIR = PORT_OUT;
	I2C_SDA_DIR = PORT_OUT;

	// Configure LCD port 
	LCD_D7_DIR = PORT_OUT;	
	LCD_D6_DIR = PORT_OUT;
	LCD_D5_DIR = PORT_OUT;
	LCD_D4_DIR = PORT_OUT;
	LCD_E_DIR  = PORT_OUT;
	LCD_RW_DIR = PORT_OUT;
	LCD_RS_DIR = PORT_OUT;

	//KEYPAD PORT
	KEYPAD_PORT = PORT_IN;

	// Control switches	
	KEYPAD_PORT_0_DIRECTION	= PORT_IN;

	//LAMP CONTROL
		
	BUZZER_DIR 			= PORT_OUT;			// Configure Tower Lamp controls
	BUZZER 				= SWITCH_OFF;

	LAMP_GREEN_DIR			= PORT_OUT;
	LAMP_GREEN 				= SWITCH_OFF;
	
	LAMP_RED_DIR				= PORT_OUT;
	LAMP_RED 				= SWITCH_OFF;

	LAMP_YELLOW_DIR			= PORT_OUT;
	LAMP_YELLOW 				= SWITCH_OFF;

    // Enable internal PORTB pull-ups
    INTCON2bits.RBPU = 0;

	

}



/*
*------------------------------------------------------------------------------
* Private Functions
*------------------------------------------------------------------------------
*/

/*
*  End of device.c
*/
