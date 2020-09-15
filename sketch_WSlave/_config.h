#ifndef _config_H_
#define _config_H_

#include "constant.h"



// board metadata
// =========================
// a number between 2 and 253
#define WS_DEVICE_NUMBER    12
#define WS_DEVICE_NAME      "webrelay"
#define WS_RELAY_NB_MAX     32
#define WS_STORAGE          WS_STORAGE_EEPROM
#define WS_LOG_LEVEL        WS_LOG_LEVEL_OFF
#define WS_LOWPOWER         ( WS_LOWPOWER_ALL - WS_LOWPOWER_NO_LED )
#define WS_SLEEP_TIME       WS_SLEEP_TIME_MAX
// =========================


// ITF conf
// =========================
#define WS_INTERFACE        WS_INTERFACE_SERIAL
#define WS_VERBOSE          WS_VERBOSE_ALL
#define WS_ACL_ALLOW        WS_ACL_ALLOW_LOCK
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WS_SERIAL_ID        3
#define WS_SERIAL_SPEED     9600
#define WS_USB_SPEED        9600
// =========================


// WEB conf
// =========================
#define WS_DHCP_TIMEOUT_MS  10000
#define WS_WEB_PORT         80
#define WS_MAC_TYPE         WS_MAC_TYPE_STATIC
#define WS_BONJOUR_MODE     WS_BONJOUR_MODE_STATIC
// =========================


// Wifi conf
// =========================
#define WS_WIFI_SSID        "mySSID"
#define WS_WIFI_PASSWORD    "myPassword"
// =========================



#endif // config_H_
