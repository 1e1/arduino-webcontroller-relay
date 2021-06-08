#ifndef _config_H_
#define _config_H_

#include "constant.h"



// board metadata
// =========================
#define WM_PIN_SAFEMODE     4 /* D2 /* GPIO4 */
#define WM_PIN_CONFIG       5 /* D1 /* GPIO5 */
#define WM_RELAY_NB_MAX     32
#define WM_LOG_LEVEL        WM_LOG_LEVEL_OFF
#define WM_COMPONENT        ( WM_COMPONENT_MDNS + WM_COMPONENT_API )
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WM_SERIAL_SPEED     115200
// =========================


// WEB conf
// =========================
#define WM_WEB_SERVER_SECURE    WM_WEB_SERVER_SECURE_YES
// =========================



#endif // _config_H_
