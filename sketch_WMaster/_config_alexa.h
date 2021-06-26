#ifndef _config_H_
#define _config_H_

#include "constant.h"



// board metadata
// =========================
#define WM_PIN_SAFEMODE     2  /* GPIO0: GND if switch#7 ON | GPIO02: top GND | GIO16: bottom GND */
#define WM_PIN_CONFIG       12 /* GPIO12; face to GND */
#define WM_LOG_LEVEL        WM_LOG_LEVEL_OFF
#define WM_LED_ON           HIGH
#define WM_USE_ASYNC        1
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WM_SERIAL_SPEED     115200
// =========================


// WEB conf
// =========================
#define WM_WEB_SERVER_SECURE    WM_WEB_SERVER_SECURE_NO
// =========================



#endif // _config_H_
