#ifndef config_H_
#define config_H_

#include "constant.h"


#define NB_RELAYS_MAX           32

#define WS_MODE_SERIAL             WS_MODE_SERIAL_ETHERNET
#define WS_MODE_VERBOSE            WS_MODE_VERBOSE_NONE
#define WS_RELAY_WIRING            WS_RELAY_WIRING_NC
#define WS_TYPE_MAC                WS_TYPE_MAC_RANDOM
#define WS_MODE_BONJOUR            WS_MODE_BONJOUR_NONE
#define WS_ACL_ALLOW               WS_ACL_ALLOW_NONE
#define WS_DATA_STORAGE            WS_DATA_STORAGE_EEPROM
#define WS_LOG_LEVEL               WS_LOG_LEVEL_OFF



// board metadata
// =====================
// a number between 2 and 253
#define DEVICE_NUMBER   12
#define DEVICE_NAME     "webrelay"
#define LOOP_SLEEP_MS   0
// =====================


// USB conf
// =====================
#define USB_SPEED       9600
// =====================


// ETH conf
// =====================
#define DHCP_TIMEOUT_MS 3000
#define WEB_PORT        80
// =====================


#endif // config_H_
