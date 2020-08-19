#ifndef config_H_
#define config_H_

#include "Arduino.h"
#include "macro.h"


#define NB_RELAYS               32

// MODE_SERIAL = MODE_SERIAL_USB | MODE_SERIAL_ETHERNET | MODE_SERIAL_LCD | MODE_SERIAL_ALL
#define MODE_SERIAL             MODE_SERIAL_USB + MODE_SERIAL_ETHERNET

// MODE_VERBOSE = MODE_VERBOSE_NONE | MODE_VERBOSE_HELP | MODE_VERBOSE_LIST | MODE_VERBOSE_WEBAPP | MODE_VERBOSE_ALL
#define MODE_VERBOSE            MODE_VERBOSE_ALL

// PIN_RELAY_WIRING = PIN_RELAY_WIRING_NC |PIN_RELAY_WIRING_NO
#define RELAY_WIRING            RELAY_WIRING_NC



// a number between 1 and 254
// =====================
#define DEVICE_NUMBER   12
// =====================


// USB conf
// =====================
#define USB_SPEED       115200
// =====================


// ETH conf
// =====================
#define DHCP_TIMEOUT_MS 3000
#define WEB_PORT        80
// =====================


#endif // config_H_
