#ifndef config_H_
#define config_H_

#include "Arduino.h"
#include "macro.h"


#define NB_RELAYS               32

#define MODE_SERIAL             MODE_SERIAL_ETHERNET
#define MODE_VERBOSE            MODE_VERBOSE_NONE
#define RELAY_WIRING            RELAY_WIRING_NC
#define TYPE_MAC                TYPE_MAC_RANDOM
#define MODE_BONJOUR            MODE_BONJOUR_NONE
#define ACL_ALLOW               ACL_ALLOW_NONE
#define DATA_STORAGE            DATA_STORAGE_EEPROM



// board metadata
// =====================
// a number between 2 and 253
#define DEVICE_NUMBER   12
#define DEVICE_NAME     "webrelay"
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
