#ifndef config_H_
#define config_H_

#include "_config.h"



// board metadata
// =========================
// a number between 2 and 253
#ifndef WM_PIN_SAFEMODE
#define WM_PIN_SAFEMODE     A0
#endif
#ifndef WM_PIN_CONFIG
#define WM_PIN_CONFIG       A0
#endif
#ifndef WM_WAKE_UP_PIN
#define WM_WAKE_UP_PIN      16
#endif
#ifndef WM_COMPONENT
#define WM_COMPONENT        WM_COMPONENT_ALL
#endif
#ifndef WS_LOG_LEVEL
#define WS_LOG_LEVEL        WM_LOG_LEVEL_ALL
#endif
#ifndef WM_USE_LED
#define WM_USE_LED          0
#endif
#ifndef WM_LED_ON
#define WM_LED_ON           LOW
#endif
#ifndef WM_LED_OFF
  #if WM_LED_ON == LOW
    #define WM_LED_OFF      HIGH
  #else
    #define WM_LED_OFF      LOW
  #endif
#endif
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#ifndef WM_SERIAL    
#define WM_SERIAL           Serial
#endif
#ifndef WM_SERIAL_SPEED    
#define WM_SERIAL_SPEED     115200
#endif
#ifndef WM_USB_SPEED
#define WM_USB_SPEED        9600
#endif
// =========================


// WEB conf
// =========================
#ifndef WM_WEB_SERVER_SECURE
#define WM_WEB_SERVER_SECURE    WM_WEB_SERVER_SECURE_NO
#endif
#ifndef WM_WEB_FILE_EXT
    #if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
    #define WM_WEB_FILE_EXT     WM_WEB_FILE_EXT_BR
    #else
    #define WM_WEB_FILE_EXT     WM_WEB_FILE_EXT_GZ
    #endif
#endif
#ifndef WM_WEB_PORTAL
#define WM_WEB_PORTAL           WM_WEB_PORTAL_NO
#endif
// =========================


// Wifi conf
// =========================
#ifndef WM_WIFI_SSID
#define WM_WIFI_SSID                    "HelloWorld"
#endif
#ifndef WM_WIFI_PASSWORD
//#define WM_WIFI_PASSWORD              "Open Sesame!"
#endif
#ifndef WM_WIFI_CONNEXION_TIMEOUT_MS
#define WM_WIFI_CONNEXION_TIMEOUT_MS    30000
#endif
// =========================



#endif // config_H_
