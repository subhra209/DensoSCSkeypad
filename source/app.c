#include "app.h"


rom static UINT24 STAGING_TIME[MAX_PLANTS][MAX_TRUCKS]={{84600,21900,23700,31800,34200,42600,44100,46200,53100,55500,57300,64200,66000,67800,75900,77700},
													   {30000,38700,51300,62400,73800,82500}};
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
	UINT24 rtcValue;
	UINT8 truckFlag[MAX_PLANTS][MAX_TRUCKS];

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

	LAMP_GREEN  = FALSE;
	LAMP_YELLOW = FALSE;
	LAMP_RED 	= FALSE;
	BUZZER  	= FALSE;
}

void APP_task(void)
{
	UINT8 i;

	//Read RTC data and store it in buffer
	ReadRtcTimeAndDate(readTimeDateBuffer); 

	//calculate and store the rtc value in the form of SEC
	app.rtcValue = (UINT24)( ( readTimeDateBuffer[2] * 3600 ) +( readTimeDateBuffer[1] * 60 ) + readTimeDateBuffer[0] );

	

	if( LinearKeyPad_getKeyState (0) == TRUE )
	{
		LAMP_GREEN  = FALSE;
 		LAMP_YELLOW = FALSE;
		LAMP_RED 	= FALSE;
		BUZZER  	= FALSE;
	}

	for( i = 0 ; i < 16 ; i++)
	{

		if( app.truckFlag[0][i]  == FALSE )
		{
			if ( ( app.rtcValue >= ( STAGING_TIME[0][i]  - 180) ) && ( app.rtcValue < ( STAGING_TIME[0][i]) ) )
			{
				LAMP_GREEN  = TRUE;
				LAMP_YELLOW = FALSE;
				LAMP_RED 	= FALSE;
				BUZZER  	= FALSE;
				break;
	
			}
			else if( ( app.rtcValue >= ( STAGING_TIME[0][i] ) ) && ( app.rtcValue < ( STAGING_TIME[0][i] + 600 ) ))
			{
				LAMP_GREEN  = FALSE;
				LAMP_YELLOW = TRUE;
				LAMP_RED 	= FALSE;
				BUZZER  	= FALSE;
				break;
			}
			else if(( app.rtcValue >= ( STAGING_TIME[0][i] + 600 ) ))
			{
				LAMP_GREEN  = FALSE;
				LAMP_YELLOW = FALSE;
				LAMP_RED 	= TRUE;
				BUZZER  	= TRUE;
				app.truckFlag[0][i]  = TRUE;
				break;
			}
		}
		else
			continue;
	}
	for( i = 0 ; i < 6 ; i++)
	{
		if( app.truckFlag[1][i]  == FALSE )
		{
			if( ( app.rtcValue >= ( STAGING_TIME[1][i]  - 180) ) && ( app.rtcValue < ( STAGING_TIME[1][i]) ) )
			{
				LAMP_GREEN  = TRUE;
				LAMP_YELLOW = FALSE;
				LAMP_RED 	= FALSE;
				BUZZER  	= FALSE;
				break;
			}
			else if( ( app.rtcValue >= ( STAGING_TIME[1][i] ) ) && ( app.rtcValue < ( STAGING_TIME[1][i] + 600 ) ))
			{
				LAMP_GREEN  = FALSE;
				LAMP_YELLOW = TRUE;
				LAMP_RED 	= FALSE;
				BUZZER  	= FALSE;
				break;
			}
			else if(( app.rtcValue >= ( STAGING_TIME[1][i] + 600 ) ) )
			{
				LAMP_GREEN  = FALSE;
				LAMP_YELLOW = FALSE;
				LAMP_RED 	= TRUE;
				BUZZER  	= TRUE;
				app.truckFlag[1][i]  = TRUE;
				break;
			}
		}
		else
			continue;
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
	UINT24 hr;
	UINT16 min  ;
	UINT8 plant = data[1];
	UINT8 truck = ( ( data[2]- '0' )* 10 )+ ( data[3] - '0')  ;

	hr	= (UINT24) ( ( ( data[4]- '0' )* 10 )+ ( data[5] - '0') )* 3600 ;
	min = (UINT16)  ( ( ( data[6]- '0' )* 10 )+ ( data[7] - '0') )* 60;
	hr 	= hr + (UINT24)min;


	STAGING_TIME[plant][truck] = hr;


}


void APP_updateRTC(far UINT8* data)
{
	writeTimeDateBuffer[0] = 0;
	writeTimeDateBuffer[1] = ((data[2] - '0') << 4) | (data[3] - '0'); //store minutes
	writeTimeDateBuffer[2] = ((data[0] - '0') << 4) | (data[1] - '0'); //store houres

	WriteRtcTimeAndDate(writeTimeDateBuffer);  //update RTC
}