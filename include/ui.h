

typedef enum
{

	UI_IDEAL= 0 ,
	UI_PASSWORD,
	UI_PLANT,
	UI_TRUCK,
	UI_TIME

}UI_STATE;




	
	
enum
{
	UI_MSG_IDEAL = 0,
	UI_MSG_PASSWORD,
	UI_MSG_PLANT_NO,
	UI_MSG_TRUCK_NO,
	UI_MSG_TIME
	
};
		

extern const rom UINT8 *UI_MSG[];

void UI_init(void);
void UI_task(void);

