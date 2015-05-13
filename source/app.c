#include "app.h"


typedef struct _APP
{
	UINT8 index;
	UINT8 entries[MAX_ENTRIES];
	UINT8 updateFlag;
	UINT8 password[5];
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
	if( app.updateFlag > 0) 
	{
		

		app.updateFlag--;
	}


}
BOOL APP_checkPassword(UINT8 *password)
{

	if( strcmp(app.password , password) )
		return FALSE;
	return TRUE;
}

void APP_updateData(far UINT8* data)
{
	strcpy(app.entries , data);
	app.updateFlag++;
}