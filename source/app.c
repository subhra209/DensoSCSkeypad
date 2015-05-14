#include "app.h"


//rom const UINT16 STAGING_TIME[MAX_PLANTS][MAX_TRUCKS]={}
/*
*------------------------------------------------------------------------------
* Private Functions	Prototypes
*------------------------------------------------------------------------------
*/
UINT8 readTimeDateBuffer[6] = {0};
UINT8 writeTimeDateBuffer[] = {0X50, 0X59, 0X00, 0X03, 0x027, 0X12, 0X13};


typedef struct _APP
{
	UINT8 index;
	UINT8 entries[MAX_ENTRIES];
	UINT8 password[5];
	UINT16 rtcValue;

}APP;

#pragma idata APP_DATA
APP app = {0};
#pragma idata



void APP_init(void)
{

	app.password[0] = '1';
	app.password[1] = '0';
	app.password[2] = '0';
	app.password[3] = '3';
	app.password[4] = '\0';
}

void APP_task(void)
{

		//Read RTC data and store it in buffer
		ReadRtcTimeAndDate(readTimeDateBuffer); 

		//calculate and store the rtc value in the form of SEC
		app.rtcValue = (UINT16)((	readTimeDateBuffer[2] * 60 ) +( readTimeDateBuffer[1] * 60 ) );



	if( LinearKeyPad_getKeyState (0) == TRUE )
	{
		LAMP_GREEN  = FALSE;
		LAMP_YELLOW = FALSE;
		LAMP_RED 	= FALSE;
		BUZZER  	= FALSE;
	}



}
BOOL APP_checkPassword(UINT8 *password)
{

	if( strcmp(app.password , password) )
		return FALSE;
	return TRUE;
}

void APP_updateStaging(far UINT8* data)
{


}


void APP_updateRTC(far UINT8* data)
{
	writeTimeDateBuffer[0] = 0;
	writeTimeDateBuffer[1] = ((data[2] - '0') << 4) | (data[3] - '0'); //store minutes
	writeTimeDateBuffer[2] = ((data[0] - '0') << 4) | (data[1] - '0'); //store houres

	WriteRtcTimeAndDate(writeTimeDateBuffer);  //update RTC
}