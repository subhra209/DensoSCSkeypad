#include "timer.h"

typedef struct _TMR
{
	unsigned int reload;
	void (*func)(void);
}TMR;

TMR tmr[3] = { 0 , 0 };



unsigned long heartBeatCount  =0 ;
UINT16 keypadUpdateCount  =0 ;
UINT16 comUpdateCount = 0;
UINT16 appUpdateCount = 0;
UINT16 uiUpdateCount = 0;
INT16 timeStampUpdateCount = TIMESTAMP_DURATION;
UINT32 AppTimestamp = 0;

/*
*------------------------------------------------------------------------------
* void TMR0_ISR(void)
*
* Summary	: Timer0 ISR for periodi tick for schedular base
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#pragma code
#pragma interrupt TMR0_ISR
void TMR0_ISR(void)
{
  	/*
   	* Clears the TMR0 interrupt flag to stop the program from processing the
   	* same interrupt multiple times.
   	*/
  	INTCONbits.TMR0IF = 0;

	++heartBeatCount;
	++keypadUpdateCount;
	++comUpdateCount;
	++appUpdateCount;
	++uiUpdateCount;

	--timeStampUpdateCount;

	if( timeStampUpdateCount <= 0 )
	{
		AppTimestamp++;
		timeStampUpdateCount = TIMESTAMP_DURATION;
	}

	if(tmr[0].func != 0 )
		(*(tmr[0].func))();


	// Reload value for 1ms overflow
	WriteTimer0(tmr[0].reload);
}
#pragma code

/*
*------------------------------------------------------------------------------
* void TMR1_ISR(void)
*
* Summary	: Timer1 ISR for periodi tick for multiplexed display refresh
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#pragma code
#pragma interrupt TMR1_ISR
void TMR1_ISR(void)
{
  	/*
   	* Clears the TMR1 interrupt flag to stop the program from processing the
   	* same interrupt multiple times.
   	*/
  	PIR1bits.TMR1IF = 0;
	// Reload value for 1ms overflow
	WriteTimer1(tmr[1].reload);
	if(tmr[1].func != 0 )
		(*(tmr[1].func))();

}
#pragma code

/*
*------------------------------------------------------------------------------
* void TMR2_ISR(void)
*
* Summary	: Timer2 ISR for periodi tick for multiplexed display refresh
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
#pragma code
#pragma interrupt TMR2_ISR
void TMR2_ISR(void)
{
  	/*
   	* Clears the TMR1 interrupt flag to stop the program from processing the
   	* same interrupt multiple times.
   	*/
  	PIR1bits.TMR2IF = 0;
	// Reload value for 1ms overflow
//	WriteTimer2(tmr[2].reload);
	PR2 = tmr[2].reload;
	if(tmr[2].func != 0 )
		(*(tmr[2].func))();

}
#pragma code

/*
*------------------------------------------------------------------------------
* void TMR0_init(void)

* Summary	: Initialize timer0 for schedular base
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void TMR0_init(UINT16 reload , void (*func)(void))
{
	// Enable the TMR0 interrupt,16bit counter
	// with internal clock,No prescalar.
   	OpenTimer0(TIMER_INT_ON & T0_SOURCE_INT & T0_16BIT & T0_PS_1_1);

	
	tmr[0].reload = reload;
	tmr[0].func = func;

	// Reload value for 5ms overflow
	WriteTimer0(reload);
}

/*
*------------------------------------------------------------------------------
* void TMR1_init(void)

* Summary	: Initialize timer1 for display refresh
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void TMR1_init(unsigned int reload , void (*func)())
{
	UINT8 config, config1;

	config = (TIMER_INT_ON & T1_16BIT_RW & T1_SOURCE_FOSC_4 & T1_PS_1_1 & T1_OSC1EN_OFF & T1_SYNC_EXT_OFF);
	config1 = (TIMER_GATE_OFF & TIMER_GATE_POL_LO & TIMER_GATE_TOGGLE_OFF & TIMER_GATE_1SHOT_OFF & TIMER_GATE_SRC_T1GPIN & TIMER_GATE_INT_OFF);
	// Enagle TMR1 interrrupt,16 bit counter, with internal clock, No prescalar
	OpenTimer1(config, config1 );


	// Reload value for 1ms overflow
	WriteTimer1(reload);
	// Make timer1 interrupt high priority
  	IPR1bits.TMR1IP = 1;
	PIE1bits.TMR1IE = 1;

 	T1CON         = 0x01;

	tmr[1].reload = reload;
	tmr[1].func = func;

}

/*
*------------------------------------------------------------------------------
* void TMR2_init(void)

* Summary	: Initialize timer2 for display refresh
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void TMR2_init(UINT8 reload , void (*func)())
{
	T2CON = 0X0F;  //Prescalar = 1:16 Postscalar = 1:2
  	IPR1bits.TMR2IP = 1;
	PIE1bits.TMR2IE = 1;
//	WriteTimer2(reload);
	PR2 = reload;

	tmr[2].reload = reload;
	tmr[2].func = func;


}
UINT32 GetAppTime(void)
{	
	UINT32 temp;

	DISABLE_TMR0_INTERRUPT();
	temp  = AppTimestamp;
	ENABLE_TMR0_INTERRUPT();
	return temp;
}

void ResetAppTime(void)
{
	ENTER_CRITICAL_SECTION();
	AppTimestamp = 0;
	EXIT_CRITICAL_SECTION();
}


