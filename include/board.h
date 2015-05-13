

/*
*------------------------------------------------------------------------------
* board.h
*
* Include file for port pin assignments
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* File				: device.h
*------------------------------------------------------------------------------
*

*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/


/*
*------------------------------------------------------------------------------
* Include Files
*------------------------------------------------------------------------------
*/

#include <p18f46k22.h>
#include <delays.h>
#include <timers.h>
#include "typedefs.h"
#include "config.h"



/*
*------------------------------------------------------------------------------
* Hardware Port Allocation
*------------------------------------------------------------------------------
*/


//Heart Beat
#define 	HEART_BEAT					PORTEbits.RE0
#define 	HEART_BEAT_DIRECTION		TRISEbits.TRISE0

// I2C Communication
#define 	I2C_SCL					PORTCbits.RC3
#define 	I2C_SCL_DIR 			TRISCbits.RC3
#define 	I2C_SDA					PORTCbits.RC4
#define 	I2C_SDA_DIR 			TRISCbits.RC4



// LCD Association
#define		LCD_DAT4_PORT			LATD

#define		LCD_D7					PORTDbits.RD7 			
#define		LCD_D7_DIR				TRISDbits.TRISD7
#define		LCD_D6					PORTDbits.RD6 			
#define		LCD_D6_DIR				TRISDbits.TRISD6
#define		LCD_D5					PORTDbits.RD5 			
#define		LCD_D5_DIR				TRISDbits.TRISD5
#define		LCD_D4					PORTDbits.RD4			
#define		LCD_D4_DIR				TRISDbits.TRISD4
#define		LCD_E					PORTDbits.RD3 			
#define		LCD_E_DIR				TRISDbits.TRISD3
#define		LCD_RW					PORTDbits.RD2 			
#define		LCD_RW_DIR				TRISDbits.TRISD2
#define		LCD_RS					PORTDbits.RD1			
#define		LCD_RS_DIR				TRISDbits.TRISD1

/* Keypad Association */
#define		KEYPAD_PORT				PORTB
#define		KEYPAD_PORT_DIR			TRISB
#define		KEYPAD_DEC_INT			PORTBbits.RB7		
#define		KEYPAD_DEC_INT_DIR		TRISBbits.TRISB7
#define		KEYPAD_BCD3				PORTBbits.RB3		
#define		KEYPAD_BCD3_DIR			TRISBbits.TRISB3
#define		KEYPAD_BCD2				PORTBbits.RB4		
#define		KEYPAD_BCD2_DIR			TRISBbits.TRISB4
#define		KEYPAD_BCD1				PORTBbits.RB2		
#define		KEYPAD_BCD2_DI1			TRISBbits.TRISB2
#define		KEYPAD_BCD0				PORTBbits.RB5		
#define		KEYPAD_BCD2_DI0			TRISBbits.TRISB5

//Tower Lamp Association

#define LAMP_GREEN					PORTAbits.RA0
#define LAMP_GREEN_DIR				TRISAbits.TRISA0

#define LAMP_YELLOW 				PORTAbits.RA1 
#define LAMP_YELLOW_DIR				TRISAbits.TRISA1


#define LAMP_RED					PORTAbits.RA2
#define LAMP_RED_DIR				TRISAbits.TRISA2

#define BUZZER 						PORTAbits.RA3 
#define BUZZER_DIR					TRISAbits.TRISA3



// Control switches

#define KEYPAD_PORT_0				PORTEbits.RE1
#define KEYPAD_PORT_0_DIRECTION		TRISEbits.TRISE1


	
/*
*------------------------------------------------------------------------------
* Public Defines
*------------------------------------------------------------------------------
*/

/*
*------------------------------------------------------------------------------
* Public Macros
*------------------------------------------------------------------------------
*/

#define DEVICE_ID			(0x01)


#define PERIPHERAL_CLOCK			(SYSTEM_CLOCK / 4)UL

#define PERIPHERAL_CLOCK_PERIOD 	(1 / PERIPHERAL_CLOCK)UL



// Direction controle bit is processor specific ,
#define PORT_OUT				(BOOL)(0)
#define PORT_IN					(0XFF)

#define OFF_FOREVER				(BOOL)(0)
#define LOOP_FOREVER			(BOOL)(1)

#define SWITCH_OFF				(BOOL)(0)
#define SWITCH_ON				(BOOL)(1)


#define DISPLAY_DISABLE			(BOOL)(1)
#define DISPLAY_ENABLE			(BOOL)(0)
void EnableInterrupts(void);

/*
#ifdef __74LS374__
#undef DISPLAY_DISABLE
#undef DISPLAY_ENABLE

#define DISPLAY_DISABLE			(BOOL)(0)
#define DISPLAY_ENABLE			(BOOL)(1)

#endif 			//__74LS374__
*/


#define GetSystemClock()		(SYSTEM_CLOCK)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)
#define GetPeripheralClock()	GetInstructionClock()


#define BUZZER_ON()			BUZ_OP = SWITCH_ON;
#define BUZZER_OFF()		BUZ_OP = SWITCH_OFF;

// Disable global interrupt bit.
#define ENTER_CRITICAL_SECTION()	INTCONbits.GIE = 0;

// Enable global interrupt bit.
#define EXIT_CRITICAL_SECTION()		INTCONbits.GIE = 1;

#define ENABLE_GLOBAL_INT()			EXIT_CRITICAL_SECTION()


#define DISABLE_ADC_INTERRUPT()		PIE1bits.ADIE = 0
#define ENABLE_ADC_INTERRUPT()		PIE1bits.ADIE = 1

#define DISABLE_INT0_INTERRUPT()	INTCONbits.INT0IE = 0
#define ENABLE_INT0_INTERRUPT()		INTCONbits.INT0IE = 1
#define CLEAR_INTO_INTERRUPT()		INTCONbits.INT0IF = 0


#define DISABLE_TMR0_INTERRUPT()	INTCONbits.TMR0IE = 0
#define ENABLE_TMR0_INTERRUPT()		INTCONbits.TMR0IE = 1

#define DISABLE_TMR1_INTERRUPT()	PIE1bits.TMR1IE = 0
#define ENABLE_TMR1_INTERRUPT()		PIE1bits.TMR1IE = 1


#define DISABLE_UART_TX_INTERRUPT()	PIE1bits.TXIE = 0
#define ENABLE_UART_TX_INTERRUPT()	PIE1bits.TXIE = 1

#define DISABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 0
#define ENABLE_UART_RX_INTERRUPT()	PIE1bits.RCIE = 1

#define DISABLE_UART1_TX_INTERRUPT()(DISABLE_UART_TX_INTERRUPT())
#define ENABLE_UART1_TX_INTERRUPT()	(ENABLE_UART_TX_INTERRUPT())

#define DISABLE_UART1_RX_INTERRUPT()(DISABLE_UART_RX_INTERRUPT())
#define ENABLE_UART1_RX_INTERRUPT()	(ENABLE_UART_RX_INTERRUPT())

#define DISABLE_UART2_TX_INTERRUPT() PIE3bits.TX2IE = 0
#define ENABLE_UART2_TX_INTERRUPT()	 PIE3bits.TX2IE = 1

#define DISABLE_UART2_RX_INTERRUPT() PIE3bits.RC2IE = 0
#define ENABLE_UART2_RX_INTERRUPT()	 PIE3bits.RC2IE = 1



#define HOLD_ATKBD_CLOCK()		    ATKBD_CLK_DIR= PORT_OUT;\
									ATKBD_CLK = 0;

#define RELEASE_ATKBD_CLOCK()	  	ATKBD_CLK_DIR = PORT_IN;\
									ATKBD_CLK = 1;

#define SET_INT0_FALLING_EDGE_TRIGGER()	INTCON2bits.INTEDG0 = 0

#define	ATKBD_INTERRUPT_EDGE	INTCON2.INTEDG0		

#define ENB_485_TX()	TX_EN = 1;
#define ENB_485_RX()	TX_EN = 0

#define Delay10us(us)		Delay10TCYx(((GetInstructionClock()/1000000)*(us)))
#define DelayMs(ms)												\
	do																\
	{																\
		unsigned int _iTemp = (ms); 								\
		while(_iTemp--)												\
			Delay1KTCYx((GetInstructionClock()+999999)/1000000);	\
	} while(0)

/*
*------------------------------------------------------------------------------
* Public Data Types
*------------------------------------------------------------------------------
*/

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

extern void BRD_init(void);


/*
*  End of board.h
*/