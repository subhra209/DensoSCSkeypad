#include "board.h"
#include "linearkeypad.h"
#include "rtc_driver.h"
#include <string.h>

extern void APP_init(void);
extern void APP_task(void);

void APP_updateStaging(far UINT8* data);
void APP_updateRTC(far UINT8* data);
BOOL APP_checkPassword(UINT8 *password);




