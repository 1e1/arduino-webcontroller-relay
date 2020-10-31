#ifndef config_H_
#define config_H_

#include "_config.h"



// board metadata
// =========================
// a number between 2 and 253
#ifndef WS_DEVICE_NUMBER
#define WS_DEVICE_NUMBER    12
#endif
#ifndef WS_DEVICE_NAME
#define WS_DEVICE_NAME      "webrelay"
#endif
#ifndef WS_RELAY_NB_MAX
#define WS_RELAY_NB_MAX     4
#endif
#ifndef WS_STORAGE
#define WS_STORAGE          WS_STORAGE_NONE
#endif
#ifndef WS_LOG_LEVEL
#define WS_LOG_LEVEL        WS_LOG_LEVEL_ALL
#endif
#ifndef WS_LOWPOWER
#define WS_LOWPOWER         ( WS_LOWPOWER_ALL - WS_LOWPOWER_NO_LED )
#endif
#ifndef WS_IDLE_TIME
#define WS_IDLE_TIME        T_64MS    
#endif
#ifndef WS_SLEEP_TIME
#define WS_SLEEP_TIME       T_8S    
#endif
#ifndef WS_IDLE_LOOP
#define WS_IDLE_LOOP        125    
#endif
// WS_IDLE_TIME * WS_IDLE_LOOP = available time after a INT
// =========================


// ITF conf
// =========================
#ifndef WS_INTERFACE
#define WS_INTERFACE        WS_INTERFACE_ALL
#endif
#ifndef WS_VERBOSE
#define WS_VERBOSE          WS_VERBOSE_ALL
#endif
#ifndef WS_ACL_ALLOW
#define WS_ACL_ALLOW        WS_ACL_ALLOW_NONE
#endif
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#ifndef WS_SERIAL_ID    
#define WS_SERIAL_ID        0
#endif
#ifndef WS_SERIAL_TIMEOUT    
#define WS_SERIAL_TIMEOUT   63
#endif
#ifndef WS_SERIAL_SPEED
#define WS_SERIAL_SPEED     9600
#endif
#if WS_SERIAL_ID == 1
#define WS_SERIAL Serial1
#elif WS_SERIAL_ID == 2
#define WS_SERIAL Serial2
#elif WS_SERIAL_ID == 3
#define WS_SERIAL Serial3
#else
#define WS_SERIAL Serial
#endif
// =========================


// USB conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#ifndef WS_USB_SPEED
#define WS_USB_SPEED        WS_SERIAL_SPEED
#endif
// =========================


// WEB conf
// =========================
#ifndef WS_DHCP_TIMEOUT_MS
#define WS_DHCP_TIMEOUT_MS  30000
#endif
#ifndef WS_WEB_PORT
#define WS_WEB_PORT         80
#endif
#ifndef WS_MAC_TYPE
#define WS_MAC_TYPE         WS_MAC_TYPE_DYNAMIC
#endif
#ifndef WS_BONJOUR_MODE
#define WS_BONJOUR_MODE     WS_BONJOUR_MODE_STATIC
#endif
#ifndef WS_HTML_COMPRESSION
#define WS_HTML_COMPRESSION WS_HTML_COMPRESSION_GZ
#endif
// =========================


// Wifi conf
// =========================
#ifndef WS_WIFI_SSID
#define WS_WIFI_SSID        WS_DEVICE_NAME
#endif
#ifndef WS_WIFI_PASSWORD
//#define WS_WIFI_PASSWORD    "Open Sesame!"
#endif
// =========================



#endif // config_H_
