#include "app.h"


/*
*------------------------------------------------------------------------------
* Private Functions	Prototypes
*------------------------------------------------------------------------------
*/
UINT8 readTimeDateBuffer[6] = {0};
UINT8 writeTimeDateBuffer[] = {0X00, 0X02, 0X06, 0X03, 0x027, 0X12, 0X13};
UINT8 displayBuffer[6] = {0};
void APP_conversion(void);
void setApptime( void );
UINT8 APP_Truckno( UINT32 data);
void updateTruckTime(UINT8 plant,UINT8 truck, UINT32 hr);

enum 
{
	IDLE = 0,
	GREEN = 1,
	YELLOW = 2,
	RED = 3
};

typedef struct _APP
{
	
	UINT8 plant;
	UINT8 truck;
	UINT8 entries[MAX_ENTRIES];
	UINT8 password[5];
	UINT32 rtcValue;
	UINT8 timeBuffer[6];
	UINT8 truckFlag[MAX_PLANTS][MAX_TRUCKS];
	UINT8 state;
	UINT8 time[6];
	UINT8 prevTime[6];


}APP;



UINT8 txBuffer[7] = {0};
#pragma idata APP_DATA
APP app = {0};

UINT32 STAGING_TIME[MAX_PLANTS][MAX_TRUCKS]={
{84600,21900,23700,31800,34200,40800,44100,46200,53100,55500,57300,64200,66000,67800,75900,77700},
													   {30000,38700,51300,62400,73800,82500}};

#pragma idata



void APP_init(void)
{

	UINT8 i , j , k;
	UINT8 temp[4] = {0};
	UINT32 time = 0;
	app.password[0] = '1';
	app.password[1] = '0';
	app.password[2] = '0';
	app.password[3] = '3';
	app.password[4] = '\0';

	for(i = 0 ; i < 22 ; i++)
	{
		for( j = 0; j < 4 ; j++)
		{
			temp[j] = (UINT32)Read_b_eep( (i*4) + j );	
			Busy_eep();
		}
		for( k = 0; k < 4 ; k++)
		{
			time <<= 8 ;
			time |= temp[k];
	
		}

		STAGING_TIME[0][i] = time;
		
	}
	for(i = 0 ; i < 6 ; i++)
	{
		for( j = 0; j < 4 ; j++)
		{
			temp [j] = (UINT32)Read_b_eep( ( (22 * 4) +(i*4)) + j );	
			Busy_eep();
		}
		for( k = 0; k < 4 ; k++)
		{
			time <<= 8 ;
			time |= temp[k];	
	
		}
		STAGING_TIME[1][i] = time;
		
	}


	LAMP_GREEN  = FALSE;
	LAMP_YELLOW = FALSE;
	LAMP_RED 	= FALSE;
	BUZZER  	= FALSE;
	app.state = IDLE;

	ReadRtcTimeAndDate(readTimeDateBuffer);

	APP_conversion();
	//calculate and store the rtc value in the form of SEC
	app.rtcValue = (UINT32) ( ( ( app.timeBuffer[3]- '0' )* 10 )+ ( app.timeBuffer[2] - '0') )* 60 + ( ( ( app.timeBuffer[1]- '0' )* 10 )+ ( app.timeBuffer[0] - '0') ) ;
	app.rtcValue += (UINT32) ( ( ( app.timeBuffer[5]- '0' )* 10 )+ ( app.timeBuffer[4] - '0') ) * 3600;	

	for( i = 0 ; i < 16 ; i++)
	{
		if(( app.rtcValue >= ( STAGING_TIME[0][i] + 600 ) ))
 		{
			app.truckFlag[0][i] = TRUE;
		}
	}

	for( i = 0 ; i < 6 ; i++)
	{
		if(( app.rtcValue >= ( STAGING_TIME[1][i] + 600 ) ))
		{
			app.truckFlag[1][i] = TRUE;
		}
	}
}

void APP_task(void)
{
	UINT8 i;
		ReadRtcTimeAndDate(readTimeDateBuffer);	//Read RTC data and store it in buffer
		APP_conversion();

		if( UI_stateStatus() == 0x00)
			setApptime();	
	

#if defined (RTC_DATA_ON_UART)
				for(i = 0; i < 7; i++)			
				{
					txBuffer[i] = readTimeDateBuffer[i];  //store time and date 
				}
				
				COM_txBuffer(txBuffer, 7);
#endif

		//calculate and store the rtc value in the form of SEC
		app.rtcValue = (UINT32) ( ( ( app.timeBuffer[3]- '0' )* 10 )+ ( app.timeBuffer[2] - '0') )* 60 
						+ ( ( ( app.timeBuffer[1]- '0' )* 10 )+ ( app.timeBuffer[0] - '0') ) ;
		app.rtcValue += (UINT32) ( ( ( app.timeBuffer[5]- '0' )* 10 )+ ( app.timeBuffer[4] - '0') ) * 3600;	


	switch(app.state)
	{
		case IDLE:
	
		for( i = 0 ; i < 16 ; i++)
		{

			if( app.truckFlag[0][i]  == FALSE )
			{
				if ( ( app.rtcValue >= ( STAGING_TIME[0][i]  - 180) ) 
						&& ( app.rtcValue < ( STAGING_TIME[0][i]) ) )
				{
					LAMP_GREEN  = TRUE;
					LAMP_YELLOW = FALSE;
					LAMP_RED 	= FALSE;
					BUZZER  	= FALSE;
					app.plant = 0;
					app.truck = i;
					app.state = GREEN;
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
				if( ( app.rtcValue >= ( STAGING_TIME[1][i]  - 180) ) 
							&& ( app.rtcValue < ( STAGING_TIME[1][i]) ) )
				{
					LAMP_GREEN  = TRUE;
					LAMP_YELLOW = FALSE;
					LAMP_RED 	= FALSE;
					BUZZER  	= FALSE;
					app.plant = 1;
					app.truck = i;
					app.state = GREEN;
					break;
				}
			
			}
			else
				continue;
		}
		break;

		case GREEN:
		if( LinearKeyPad_getKeyState (0) == TRUE )
		{
			LAMP_GREEN  = FALSE;
	 		LAMP_YELLOW = FALSE;
			LAMP_RED 	= FALSE;
			BUZZER  	= FALSE;
			app.truckFlag[app.plant][app.truck] = 	TRUE;
			app.state = IDLE;
		 	return;
		}

		 if( ( app.rtcValue >= ( STAGING_TIME[app.plant][app.truck] ) ) 
					&& ( app.rtcValue < ( STAGING_TIME[app.plant][app.truck] + 180 ) ))
		{
				LAMP_GREEN  = FALSE;
				LAMP_YELLOW = TRUE;
				LAMP_RED 	= FALSE;
				BUZZER  	= FALSE;
				app.state = YELLOW;
			
		}
		break;
			
		case YELLOW:
		if( LinearKeyPad_getKeyState (0) == TRUE )
		{
			LAMP_GREEN  = FALSE;
	 		LAMP_YELLOW = FALSE;
			LAMP_RED 	= FALSE;
			BUZZER  	= FALSE;
			app.truckFlag[app.plant][app.truck] = 	TRUE;
			app.state = IDLE;
			return;
		}
		if(( app.rtcValue >= ( STAGING_TIME[app.plant][app.truck] + 180 ) ))
		{
			LAMP_GREEN  = FALSE;
			LAMP_YELLOW = FALSE;
			LAMP_RED 	= TRUE;
			BUZZER  	= TRUE;
			
			app.state = RED;
			
		}
		break;

		case RED:
		if( LinearKeyPad_getKeyState (0) == TRUE )
		{
			LAMP_GREEN  = FALSE;
	 		LAMP_YELLOW = FALSE;
			LAMP_RED 	= FALSE;
			BUZZER  	= FALSE;
			app.truckFlag[app.plant][app.truck] = 	TRUE;
			app.state = IDLE;
			return;
		}
		break;
	}

}




BOOL APP_checkPassword(UINT8 *password)
{

	if( strcmp(app.password , password) )
		return FALSE;
	return TRUE;
}

void APP_updateRTC(far UINT8* data)
{
	writeTimeDateBuffer[0] = 0;
	writeTimeDateBuffer[1] = ((data[2] - '0') << 4) | (data[3] - '0'); //store minutes
	writeTimeDateBuffer[2] = ((data[0] - '0') << 4) | (data[1] - '0'); //store houres

	WriteRtcTimeAndDate(writeTimeDateBuffer);  //update RTC
}

void APP_conversion(void)
{
			
	app.timeBuffer[0] = (readTimeDateBuffer[0] & 0X0F) + '0';        //Seconds LSB
	app.timeBuffer[1] = ((readTimeDateBuffer[0] & 0XF0) >> 4) + '0'; //Seconds MSB
	app.timeBuffer[2] = (readTimeDateBuffer[1] & 0X0F) + '0';        //Minute LSB
	app.timeBuffer[3] = ((readTimeDateBuffer[1] & 0XF0) >> 4) + '0' ; 		//Minute MSB
	app.timeBuffer[4] = (readTimeDateBuffer[2] & 0X0F) + '0';        //Minute LSB
	app.timeBuffer[5] = ((readTimeDateBuffer[2] & 0X30) >> 4)  + '0'; 		//Minute MSB

}

void APP_updateTime(UINT8* data)
{

}
 
UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket)
{

	UINT8 i;

	UINT8 length = 0;

	return length;

}

void setApptime( void )
{
	UINT8 i;
	UINT8 msg[] = "  DENSO : ";
	for(i = 0 ; i < 4 ; i++)
		{
			app.time[i] = app.timeBuffer[5-i];
		}
		app.time[i] = '\0';

	if( strcmp(app.time, app.prevTime) )
	{
		LCD_clear( );
		i = 0;

		while( msg[i] != '\0')
		{
			LCD_putChar(msg[i]);
			i++;
		}
	
		i = 0;

		while( app.time[i] != '\0')
		{
			LCD_putChar(app.time[i]);
			i++;
		}
		strcpy(app.prevTime, app.time);
	}

}

void APP_updateStaging(far UINT8* data)
{

	UINT24 hr;
	UINT16 min  ;
	UINT8 plant = data[1] - '0';
	UINT8 truck = ( ( data[2]- '0' )* 10 )+ ( data[3] - '0')  ;

	hr	= (UINT24) ( ( ( data[4]- '0' )* 10 )+ ( data[5] - '0') )* 3600 ;
	min = (UINT16)  ( ( ( data[6]- '0' )* 10 )+ ( data[7] - '0') )* 60;
	hr 	= hr + (UINT24)min;
	
	updateTruckTime(plant,truck,hr);


}

void updateTruckTime(UINT8 plant,UINT8 truck, UINT32 hr)
{

	UINT8 i;
	UINT8* temp;
	UINT8 temp1[4] = {0};
	UINT32 time  = 0;
	UINT8 index = (truck - 1) * 4 ;

	temp = (UINT8 *)&hr;



	if( plant == 1)
	{
		for( i = 0; i < 4 ; i++)
		{
			Write_b_eep(index + i , *(temp + (3 -i) ) );	
			Busy_eep();
		}
	}
	else if(plant == 2)
	{
		for( i = 0; i < 4 ; i++)
		{
			Write_b_eep( ((22 *4)+index ) + i , *(temp + (3 -i) ) );	
			Busy_eep();
		}
	}

	if( plant == 1)
	{
		for( i = 0; i < 4 ; i++)
		{
			temp1[i] = Read_b_eep(index + i  );
			Busy_eep();	

		}
	}
	else if(plant == 2)
	{
		for( i = 0; i < 4 ; i++)
		{
			temp1[i] = Read_b_eep( (22*4 +index )+ i  );
			Busy_eep();

		}
	}

	for( i = 0; i < 4 ; i++)
	{
		time <<= 8 ;
		time |= temp1[i];
	

	}

	STAGING_TIME[plant - 1][truck - 1]  = time ;

}