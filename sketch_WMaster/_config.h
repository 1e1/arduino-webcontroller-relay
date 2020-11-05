#ifndef _config_H_
#define _config_H_

#include "constant.h"



// board metadata
// =========================
#define WM_PIN_SAFEMODE     A0 /* RXD */
#define WM_PIN_CONFIG       A0 /* GPIO12 */
#define WM_RELAY_NB_MAX     32
#define WM_LOG_LEVEL        WM_LOG_LEVEL_OFF
// =========================


// Serial conf
// =========================
// multiply by the clock divisor (ex:9600 baud at 2MHz instead of 16Mhz is 1200 baud)
#define WM_SERIAL_SPEED     115200
// =========================


// WEB conf
// =========================
#define WM_WEB_PORT             WM_WEB_PORT_DEFAULT_SECURE
#define WM_WEB_SERVER_SECURE    WM_WEB_SERVER_SECURE_YES
#define WM_WEB_FILE_EXT         WM_WEB_FILE_EXT_BR
// =========================



#endif // _config_H_
