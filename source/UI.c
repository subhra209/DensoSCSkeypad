#include "board.h"
#include "config.h"
#include "Keypad.h"
#include "lcd.h"
#include "string.h"
#include "ui.h"
#include "app.h"



typedef struct _UI
{
	UI_STATE state;
	UINT8 buffer[MAX_INPUT_CHARS+1];
	UINT8 bufferIndex;
	UINT8 input[MAX_INPUT_CHARS+1];
	UINT8 inputIndex;
}UI;



/*
*------------------------------------------------------------------------------
* Private Constants (static)
*------------------------------------------------------------------------------
*/


const rom UINT8 *UI_MSG[]=
		{"		DENSO",
		"PASSWORD:"
		"SETTING:",
		"STAGING TIME",
		"CLOCK TIME",
		"PLANT NO:",
		"TRUCK NO:",
		"TIME(HHMM):"

		};

const rom UINT8 keyMap[MAX_KEY] = { '1','2','3','\x0A',					//	1		2		3			logon
									 '4','5','6','\x0B',				//	4		5		6			logoff
									 '7','8','9','\x08',				//	7		8		9			backspace
									 '*','0','#','\x0C' } ;				//	clock	0		staging		enter




#pragma idata UI_DATA
UI ui = {0};

//#pragma idata


/*
*------------------------------------------------------------------------------
* Public Functions
*------------------------------------------------------------------------------
*/

UINT8 mapKey(UINT8 scancode, UINT8 duration);
UINT8 updateValue(void);
void clearUIBuffer(void);
void putUImsg(UINT8 msgIndex);
void setUImsg( UINT8 msgIndex );
void clearUIInput(void);


void UI_init(void)
{

	LCD_setBackSpace('\x08');	//Indicates LCD driver "\x08" is the symbol for backspace

	setUImsg(UI_MSG_IDEAL);

	clearUIBuffer();
	clearUIInput();
}



void UI_task(void)
{

	UINT8 keypressed = 0xFF;
	UINT8 i;
	UINT8 duration, scancode;
	UINT8 uimsg;

	//Check whether key has been pressed
	if(KEYPAD_read(&scancode, &duration) == FALSE)	
	{
		return;
	}

	//Map the key
	keypressed = mapKey(scancode,duration);				

	if( keypressed == 0xFF)
	{
		return;
	}


	switch(ui.state)
	{

		case UI_IDEAL:
		//for pressing logon
		if( keypressed == '\x0A')
		{
			//msg show password for enter password
			setUImsg(UI_MSG_PASSWORD);
			//clear both buffer
			clearUIBuffer();
			clearUIInput();
			//state change to PASSWORD
			ui.state = UI_PASSWORD;
			
		}

		break;

		case UI_PASSWORD:
		//for pressing BACKSPACE
		if( keypressed == '\x08')
		{
		
				setUImsg(UI_MSG_IDEAL);
				//clear both buffer
				clearUIBuffer();
				clearUIInput();

				ui.state = UI_IDEAL;	

		}
		//for pressing ENTER
		else if( keypressed == '\x0C')
		{
			BOOL result = FALSE;
			ui.buffer[ui.bufferIndex] = '\0';
	
		
			//wheather password is correct 
			result = APP_checkPassword(ui.buffer);
	
			if( result == TRUE )
			{
				//msg show for enter type of SETTING
				setUImsg(UI_MSG_SETTING);
				//clear ui_buffer
				clearUIBuffer();

				//state change to SETTING
				ui.state = UI_SETTING;
			}
			else
			{
				setUImsg(UI_MSG_IDEAL);
				clearUIBuffer();
				clearUIInput();
				ui.state = UI_IDEAL;
			}
		
		
		}

		else 
		{
			//store the value in ui_buffer
			ui.buffer[ui.bufferIndex] = keypressed;
			LCD_putChar('*');
			//increment  ui_buffer index
			ui.bufferIndex++;
		}
			
		break;	

		case UI_SETTING:

			switch(keypressed)
			{
				//for pressing CLOCK_SETTING
				case '*':
					putUImsg(UI_MSG_CLOCK_TIME);
					ui.input[ui.inputIndex++] = '*';
		
				break;
				//for pressing STAGING_SETTING
				case '#':
					putUImsg(UI_MSG_STAGING_TIME);
					ui.input[ui.inputIndex++] = '#';
	
				break;
				//for pressing LOGOFF
				case '\x0B':
					setUImsg(UI_MSG_IDEAL);
					//clear all buffer
					clearUIBuffer();
					clearUIInput();
					//state change to IDEAL
					ui.state = UI_IDEAL;

				break;
				//for pressing ENTER
				case '\x0C':
					if(ui.input[0] == '*')
					{
						setUImsg(UI_MSG_TIME);
						ui.state = UI_TIME;
					}
					else if (ui.input[0] = '#')
					{
						setUImsg(UI_MSG_PLANT_NO);
						//state change to PLNAT
						ui.state = UI_PLANT;
					}
				break;
				default:
				break;
						
			}
		break;

		case UI_PLANT:
		//for pressing Backspace
		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				LCD_putChar(keypressed);

				//decrement both buffer's inddex value
				ui.bufferIndex--;
			}
			else
			{
				//msg show for enter type of SETTING
				setUImsg(UI_MSG_SETTING);
				//clear all buffer
				clearUIBuffer();
				clearUIInput();

				//state change to SETTING
				ui.state = UI_SETTING;
			}

		}
		//for pressing ENTER
		else if( keypressed == '\x0C')
		{
			//if there is some value in that buffer
			if(ui.bufferIndex > 0)
			{
				//msg show for enter TRUCK_NO
				setUImsg(UI_MSG_TRUCK_NO);
				clearUIBuffer();
				//state change to TRUCK
				ui.state = UI_TRUCK;
				

			}
		}
		
		else
		{
			//store plant value in ui_buffer
			ui.buffer[ui.bufferIndex++] = keypressed;
			//show the plant value in LCD
			LCD_putChar(ui.buffer[ui.bufferIndex]);
			//store plant value in input buffer
			ui.input[ui.inputIndex++] = keypressed;

		}

		break;

		case UI_TRUCK:

		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				LCD_putChar(keypressed);
				ui.bufferIndex--;

			}
		
			else
			{
				//msg show for enter PLANT_NO				
				setUImsg(UI_MSG_PLANT_NO);
				//clear all buffer
				clearUIBuffer();
				ui.inputIndex = 1;
				//state change to PLNAT
				ui.state = UI_PLANT;
			}

		}

		else if( keypressed == '\x0C')
		{
			//if there is some value in that ui_buffer
			if(ui.bufferIndex > 0)
			{
				//store truck_no in input buffer
				updateValue();
				//msg show for enter TIME
				setUImsg(UI_MSG_TIME);
				//clear ui_buffer
				clearUIBuffer();
				//state change to TIME
				ui.state = UI_TIME;
				

			}
		}
		else
		{
			//store truk_no in ui_buffer
			ui.buffer[ui.bufferIndex] = keypressed;
			//show truck_no in LCD
			LCD_putChar(ui.buffer[ui.bufferIndex]);
			//increment  ui_buffer index
			ui.bufferIndex++;
		}
		
		break;

		case UI_TIME:
		if( keypressed == '\x08')
		{
			if(ui.bufferIndex > 0 )
			{
				LCD_putChar(keypressed);
				ui.bufferIndex--;
			}
			else
			{
				if(ui.input[0] == '*')
				{
					//msg show for enter type of SETTING
					setUImsg(UI_MSG_SETTING);
					//clear all buffer
					clearUIBuffer();
					clearUIInput();
	
					//state change to SETTING
					ui.state = UI_SETTING;
				}
				else if(ui.input[0] == '#')
				{
				//msg show for enter TRUCK_NO				
					setUImsg(UI_MSG_TRUCK_NO);
					//clear all buffer
					clearUIBuffer();
					ui.inputIndex = 2;
					//state change to TRUCK
					ui.state = UI_TRUCK;

				}
			}
		}
		else if( keypressed == '\x0C')
		{
				if(ui.input[0] == '*')
				{
					APP_updateRTC(ui.buffer);

				}
				else if(ui.input[0] == '#')
				{
					APP_updateStaging(ui.input);

				}

				//msg show for enter type of SETTING
				setUImsg(UI_MSG_SETTING);
				//clear all buffer
				clearUIBuffer();
				clearUIInput();

				//state change to SETTING
				ui.state = UI_SETTING;

		}
		else
		{
			ui.buffer[ui.bufferIndex++] = keypressed;
			ui.input[ui.inputIndex++]    = keypressed;
			LCD_putChar(ui.buffer[ui.bufferIndex]);

		}

		break;
		
		default:
		break;


	}



}

/*
*------------------------------------------------------------------------------
* UINT8 mapKey(UINT8 scancode, UINT8 duration);

* Summary	: Application specifc main routine. Initializes all port and
*			: pheriperal and put the main task into an infinite loop.
*
* Input		: scancode - posiition of key which is pressed
*			: duration - time duration of pressed key

* Output	:0xFF -      for igonre the key which presssed
*			:scanded key -value of pressed key from mapkey
*------------------------------------------------------------------------------
*/

UINT8 mapKey(UINT8 scancode, UINT8 duration)
{
	UINT8 keypressed = 0xFF;
	switch(ui.state)
	{

		case UI_IDEAL:
		keypressed = keyMap[scancode];
		
		//ONLY LOGON KEY IS VALID
		if( keypressed != '\x0A') 
				keypressed = 0xFF;

		break;

		case UI_PASSWORD:
		keypressed = keyMap[scancode];
		// max data given is 5 number
		if( ui.bufferIndex >= 5)
		{
			if((keypressed != '\x08') && (keypressed != '\x0C') )
			keypressed = 0xFF;
		}
		//enable key only for numbers , backspace and enter
		else
			if( keypressed == '*' || keypressed == '#' ||  keypressed == '\x0A' || keypressed == '\x0B' )
				keypressed = 0xFF;

		break;

		case UI_SETTING:
		keypressed = keyMap[scancode];

			if((keypressed != '\x0B')&& (keypressed != '\x08') && (keypressed != '\x0C') && (keypressed != '*')  && (keypressed != '#') )
				keypressed = 0xFF;
		break;

		case UI_PLANT:

		keypressed = keyMap[scancode];
		//max input is two digit
		if(ui.bufferIndex > 1)
		{
			keypressed = 0xFF;			
		}
		if( ui.bufferIndex == 1)
		{
			if((keypressed != '\x08') && (keypressed != '\x0C') )
			keypressed = 0xFF;
		}
		else
			if((keypressed != '\x08')&& (keypressed != '1') && (keypressed != '2') )
				keypressed = 0xFF;
		break;


		case UI_TRUCK:

		keypressed = keyMap[scancode];
		//max input is two digit
		if(ui.bufferIndex > 2)
		{
			keypressed = 0xFF;			
		}
		else if( ui.bufferIndex == 2)
		{
			if((keypressed != '\x08') && (keypressed != '\x0C') )
			keypressed = 0xFF;
		}
		else
			if( keypressed == '*' || keypressed == '#' ||  keypressed == '\x0A' || keypressed == '\x0B' )
				keypressed = 0xFF;
	
		break;

		case UI_TIME:

		keypressed = keyMap[scancode];
		//max input is two digit
		if(ui.bufferIndex > 4)
		{
			keypressed = 0xFF;			
		}
		else if( ui.bufferIndex == 4)
		{
			if((keypressed != '\x08') && (keypressed != '\x0C') )
			keypressed = 0xFF;
		}
		
		else 
				if( keypressed == '*' || keypressed == '#' ||  keypressed == '\x0A' || keypressed == '\x0B' || (keypressed == '\x0C') )
				keypressed = 0xFF;
	
		break;


		default:
		break;

	}

	return keypressed;
}

/*
*------------------------------------------------------------------------------
* UINT8 updateValue(void);

* Summary	:
*			:
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
UINT8 updateValue(void)
{
	UINT8 i,value = 0;

	if( ui.bufferIndex == 1 )
	{
		ui.input[ui.inputIndex] = '0';
		ui.inputIndex++;
		ui.input[ui.inputIndex] = ui.buffer[0];
		ui.inputIndex++;
	}

	else
	{
		ui.input[ui.inputIndex] = ui.buffer[0];
		ui.inputIndex++;
		ui.input[ui.inputIndex] = ui.buffer[1];
		ui.inputIndex++;
	}

	value = (ui.input[0]-'0')*10 + (ui.input[1]-'0');

	return value;
}

/*
*------------------------------------------------------------------------------
* void clearUIBuffer(void);

* Summary	: fill the input buffer with zero
*			: and index point to its first element
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void clearUIBuffer(void)
{
	memset(ui.buffer,0, MAX_INPUT_CHARS);
	ui.bufferIndex = 0;

}
/*
*------------------------------------------------------------------------------
* void clearUIInput(void);

* Summary	: fill the input buffer with zero
*			: and index point to its first element
*
* Input		: None
*
* Output	: None
*
*------------------------------------------------------------------------------
*/

void clearUIInput(void)
{
	memset((UINT8*)ui.input,0, MAX_INPUT_CHARS);
	ui.inputIndex = 0;
}


/*
*------------------------------------------------------------------------------
* void setUImsg( UINT8 msgIndex );

* Summary	:read msg form specific location and display 
*			:it on LCD
*
* Input		: msg index which point to that msg present in rom
*
* Output	: None
*
*------------------------------------------------------------------------------
*/
void setUImsg( UINT8 msgIndex )
{
	UINT8 i;

	const rom UINT8 *msg;

	msg = UI_MSG[msgIndex] ;

	LCD_clear();

	i = 0;
	while( msg[i] != '\0')
	{
		LCD_putChar(msg[i]);
		i++;
	}
}

/*
*------------------------------------------------------------------------------
* void putUImsg(UINT8 msgIndex)

* Summary	: read msg form specific location and display
*			:along with priviously which displayed in lcd		
*
* Input		: msg index which point to that msg present in rom
*
* Output	: none
*
*------------------------------------------------------------------------------
*/

void putUImsg(UINT8 msgIndex)
{
	UINT8 i;

	const rom UINT8 *msg;

	msg = UI_MSG[msgIndex] ;

	i = 0;
	while( msg[i] != '\0')
	{
		LCD_putChar(msg[i]);
		i++;
	}
}


		