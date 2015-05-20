#include "board.h"
#include "linearkeypad.h"
#include "rtc_driver.h"
#include "lcd.h"
#include <string.h>
#include "communication.h"

extern void APP_init(void);
extern void APP_task(void);

void APP_updateStaging(far UINT8* data);
void APP_updateRTC(far UINT8* data);
BOOL APP_checkPassword(UINT8 *password);
extern UINT8 APP_comCallBack( far UINT8 *rxPacket, far UINT8* txCode,far UINT8** txPacket);






