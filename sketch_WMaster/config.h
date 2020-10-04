#ifndef config_H_
#define config_H_

#include "constant.h"



// board metadata
// =========================
#define WM_PIN_ERASE        0 /* RXD */
#define WM_PIN_CONFIG       0 /* GPIO12 */
#define WM_STORAGE          WS_STORAGE_NONE
#define WM_LOG_LEVEL        WS_LOG_LEVEL_ALL
// =========================


// ITF conf
// =========================
#define WM_VERBOSE          WS_VERBOSE_ALL
#define WM_ACL_ALLOW        WS_ACL_ALLOW_NONE
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WM_SERIAL_SPEED     115200
#define WM_USB_SPEED        WM_SERIAL_SPEED
// =========================


// WEB conf
// =========================
#define WM_DHCP_TIMEOUT_MS  30000
#define WM_WEB_PORT         80
// =========================


// Wifi conf
// =========================
#define WM_WIFI_SSID        "HelloWorld"
#define WM_WIFI_PASSWORD    "Open Sesame!"
// =========================



#endif // config_H_
