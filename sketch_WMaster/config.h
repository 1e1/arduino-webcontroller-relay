#ifndef config_H_
#define config_H_

#include "constant.h"



// board metadata
// =========================
#define WM_PIN_SAFEMODE     0 /* RXD */
#define WM_PIN_CONFIG       0 /* GPIO12 */
#define WM_RELAY_NB_MAX     32
#define WM_LOG_LEVEL        WM_LOG_LEVEL_OFF
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WM_SERIAL           Serial
#define WM_SERIAL_SPEED     115200
#define WM_USB_SPEED        WM_SERIAL_SPEED
// =========================


// WEB conf
// =========================
#define WM_WEB_PORT         443
// =========================


// Wifi conf
// =========================
#define WM_WIFI_SSID        "HelloWorld"
// =========================



#endif // config_H_
