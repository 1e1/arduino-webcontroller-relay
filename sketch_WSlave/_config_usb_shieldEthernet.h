#ifndef _config_H_
#define _config_H_

#include "constant.h"



// board metadata
// =========================
// a number between 2 and 253
#define WS_DEVICE_NUMBER    12
#define WS_DEVICE_NAME      "relayboard"
#define WS_RELAY_NB_MAX     32
#define WS_STORAGE          WS_STORAGE_EEPROM
#define WS_LOG_LEVEL        WS_LOG_LEVEL_OFF
#define WS_LOWPOWER         ( WS_LOWPOWER_ALL - WS_LOWPOWER_NO_LED - WS_LOWPOWER_INTERRUPTS )
#define WS_SLEEP_TIME       T_2S
// =========================


// ITF conf
// =========================
#define WS_INTERFACE        ( WS_INTERFACE_USB + WS_INTERFACE_ETHERNET )
#define WS_VERBOSE          WS_VERBOSE_ALL
#define WS_ACL_ALLOW        WS_ACL_ALLOW_ALL
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WS_USB_SPEED        9600
// =========================


// WEB conf
// =========================
#define WS_DHCP_TIMEOUT_MS  10000
#define WS_WEB_PORT         80
#define WS_MAC_TYPE         WS_MAC_TYPE_STATIC
#define WS_BONJOUR_MODE     WS_BONJOUR_MODE_STATIC
#define WS_HTML_COMPRESSION WS_HTML_COMPRESSION_GZ
// =========================



#endif // config_H_
