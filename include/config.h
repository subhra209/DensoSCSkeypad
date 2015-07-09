#ifndef CONFIG_H
#define CONFIG_H

/*
*------------------------------------------------------------------------------
* config.h
*
*/

/*----------------------------------------
*	BOARD MOULE CONFIG
*----------------------------------------*/

#define MHz_64					(64000000UL)	// Hz
#define MHz_32					(32000000UL)	// Hz
#define MHz_16					(16000000UL)	// Hz
#define MHz_10					(10000000UL)	// Hz

/*----------------------------------------
*	OSCILLATOR CONFIG
*----------------------------------------*/

#define SYSTEM_CLOCK			(MHz_16)	


/*----------------------------------------
*	TIMER CONFIG
*----------------------------------------*/
//TIMER Configuration
#define TIMESTAMP_DURATION 		(200)

#define FULLSCALE_16BIT				(65535)
#define TIMER0_TIMEOUT_DURATION 	(1000UL)			//1ms			

/*----------------------------------------
*	DEVICE CONFIG
*----------------------------------------*/
#define __FACTORY_CONFIGURATION__

#define DEVICE_ADDRESS			0x01
//MMD module configuration
#define MMD_MAX_CHARS		8
#define MMD_MAX_SEGMENTS	4
#define MMD_MAX_ADDRESS		128
#define __MMD_STATIC__

#define __UART_TEST__

/*----------------------------------------
*	COM module configuration
*----------------------------------------*/

#define ACTIVE_USARTS 1
#define __BCC_XOR__
#define __RESPONSE_ENABLED__
//#define __LOOP_BACK__
#define BROADCAST_ADDRESS		0xFF
#define CMD_SOP	0xAA
#define CMD_EOP 0xBB
#define RESP_SOP	0xCC
#define RESP_EOP	0xDD
#define __NO_CHECKSUM__

enum
{
	CMD_PACKET_SIZE = 30,
	RESP_PACKET_SIZE = 30
};

#define 	RX_PACKET_SIZE		(60)	
#define 	TX_PACKET_SIZE		(60)

/*----------------------------------------
*	Display module configuration
*----------------------------------------*/
#define __DISPLAY_TEST__
#define MAX_MSG 7
//#define COMMON_CATHODE

//LCD Configuration
//#define TEST_LCD

// Enable for external eeprom access
// Comment out this line if internal eeprom access required
#define EEP_EXTERRNAL
// For uart enable
//#define EN_UART
// Enable for hardware i2c module usage
//#define USE_MSSP_I2C

/*----------------------------------------
*	RTC CONFIGURATION
*----------------------------------------*/

//#define TIME_DEBUG
//#define RTC_DATA_ON_UART
//#define RTC_DS1307
//#define RTC_DS3232


/*----------------------------------------
*	Keypad Configurations
*----------------------------------------*/


//KEYPAD Configuration
#define KEYPAD_BUFFER_LENGTH 	(20)
//#define KEYPAD_TEST
#define INPUT_SIMULATION

/*----------------------------------------
*	APP Configurations
*----------------------------------------*/
#define MAX_ENTRIES				(30)
//TOWER LAMP Configuration
//#define TOWER_LAMP_TEST

#define MAX_PLANTS 2
#define MAX_TRUCKS 22

#define TRUCK_TIME_UPDATE		0x00

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

enum
{
	MAX_INPUT_CHARS = 10,
	MAX_OUTPUT_CHARS = 3,
	MAX_KEY = 16,
	MAX_CHAR_PER_KEY = 1,
	MIN_KEYPRESS_DURATION = 20 

	
};

/*
*------------------------------------------------------------------------------
* Public Variables (extern)
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Constants (extern)
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Public Function Prototypes (extern)
*------------------------------------------------------------------------------
*/

#endif
/*
*  End of config.h
*/



