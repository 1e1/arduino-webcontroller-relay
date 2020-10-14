#ifndef _config_H_
#define _config_H_

#include "constant.h"



// board metadata
// =========================
#define WS_RELAY_NB_MAX     32
#define WS_STORAGE          WS_STORAGE_EEPROM
#define WS_LOG_LEVEL        WS_LOG_LEVEL_OFF
#define WS_LOWPOWER         WS_LOWPOWER_NONE
// =========================


// ITF conf
// =========================
#define WS_INTERFACE        ( WS_INTERFACE_USB + WS_INTERFACE_WIFI )
#define WS_VERBOSE          WS_VERBOSE_ALL
#define WS_ACL_ALLOW        WS_ACL_ALLOW_ALL
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WS_USB_SPEED        9600
// =========================


// Wifi conf
// =========================
#define WS_WIFI_SSID        "mySSID"
#define WS_WIFI_PASSWORD    "myPassword"
#define WS_WEB_PORT         80
// =========================



#endif // config_H_
