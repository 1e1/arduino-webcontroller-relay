#ifndef config_H_
#define config_H_

#include "Arduino.h"
#include "macro.h"


#define NB_RELAYS               32

// MODE_SERIAL = MODE_SERIAL_USB | MODE_SERIAL_ETHERNET | MODE_SERIAL_ALL
#define MODE_SERIAL             MODE_SERIAL_ALL

// MODE_VERBOSE = MODE_VERBOSE_NONE | MODE_VERBOSE_HELP | MODE_VERBOSE_LIST | MODE_VERBOSE_WEBAPP | MODE_VERBOSE_ALL
#define MODE_VERBOSE            MODE_VERBOSE_ALL

// PIN_RELAY_WIRING = PIN_RELAY_WIRING_NC | PIN_RELAY_WIRING_NO
#define RELAY_WIRING            RELAY_WIRING_NC

// TYPE_MAC = TYPE_MAC_STATIC | TYPE_MAC_DYNAMIC
#define TYPE_MAC                TYPE_MAC_STATIC

// MODE_BONJOUR = MODE_BONJOUR_NONE | MODE_BONJOUR_STATIC | MODE_BONJOUR_DYNAMIC
#define MODE_BONJOUR            MODE_BONJOUR_STATIC

// DATA_STORAGE = ACL_ALLOW_NONE | ACL_ALLOW_RESET
#define ACL_ALLOW               ACL_ALLOW_RESET

// DATA_STORAGE = DATA_STORAGE_NONE | DATA_STORAGE_EEPROM
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
#define DHCP_TIMEOUT_MS 30000
#define WEB_PORT        80
// =====================


#endif // config_H_
