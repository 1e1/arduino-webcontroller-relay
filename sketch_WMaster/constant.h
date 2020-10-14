#ifndef constants_H_
#define constants_H_


/* mode */
#define WM_VERBOSE_NONE             0
#define WM_VERBOSE_PORTAL           1
#define WM_VERBOSE_WEBAPP           2
#define WM_VERBOSE_ALL              ( WM_VERBOSE_PORTAL + WM_VERBOSE_WEBAPP )

#define WM_BONJOUR_MODE_NONE        0
#define WM_BONJOUR_MODE_STATIC      1
#define WM_BONJOUR_MODE_DYNAMIC     2

#define WM_ACL_ALLOW_NONE           0
#define WM_ACL_ALLOW_REBOOT         1
#define WM_ACL_ALLOW_ALL            ( WM_ACL_ALLOW_REBOOT )

#define WM_STORAGE_NONE             0
#define WM_STORAGE_SPIFFS           1

#define WM_LOG_LEVEL_OFF            0
#define WM_LOG_LEVEL_FATAL          10
#define WM_LOG_LEVEL_ERROR          20
#define WM_LOG_LEVEL_WARN           30
#define WM_LOG_LEVEL_INFO           40
#define WM_LOG_LEVEL_DEBUG          50
#define WM_LOG_LEVEL_TRACE          60
#define WM_LOG_LEVEL_ALL            70

#define WM_LOWPOWER_NONE            0
#define WM_LOWPOWER_DOWNCLOCK       1   /* (Timer0,) Timer1, Timer2 */
#define WM_LOWPOWER_NO_ADC          2   /* No Analog */
#define WM_LOWPOWER_NO_SPI          4   /* No Ethernet Shield */
#define WM_LOWPOWER_NO_TWI          8   /* No I2C link */
#define WM_LOWPOWER_NO_USART        16  /* No Serial */
// #define WM_LOWPOWER_NO_INTERRUPT    32  /* noInterrupts() */
#define WM_LOWPOWER_HAS_DELAY       64
#define WM_LOWPOWER_NO_LED          128
#define WM_LOWPOWER_INTERRUPTS      256
#define WM_LOWPOWER_AUTOSLEEP       512
#define WM_LOWPOWER_ALL             ( WM_LOWPOWER_DOWNCLOCK + WM_LOWPOWER_NO_ADC + WM_LOWPOWER_NO_SPI + WM_LOWPOWER_NO_TWI + WM_LOWPOWER_NO_USART + WM_LOWPOWER_HAS_DELAY + WM_LOWPOWER_NO_LED + WM_LOWPOWER_INTERRUPTS + WM_LOWPOWER_AUTOSLEEP )
#define WM_LOWPOWER_WAKEUP_BOD_MS   65
#define WM_WAKEUP_PCINT_SERIAL0     PCINT8
#define WM_WAKEUP_PCINT_SERIAL1     INT2
#define WM_WAKEUP_PCINT_SERIAL3     PCINT9
#define WM_SLEEP_TIME_NONE          T_NONE
#define WM_SLEEP_TIME_MIN           T_16MS
#define WM_SLEEP_TIME_MAX           T_8S
#define WM_SLEEP_TIME_INFINITE      T_INFINITE



#define WM_CONFIG_KEY_PATH      "_KEY.txt"
#define WM_CONFIG_CERT_PATH     "_CSR.txt"
#define WM_CONFIG_GLOBAL_PATH   "global.json"
#define WM_CONFIG_WIFI_PATH     "wifis.json"
#define WM_CONFIG_RELAY_PATH    "relays.json"
#define WM_WEB_INDEX_PATH       "index.br"
#define WM_WEB_PORTAL_PATH      "portal.br"

#define WM_CONFIG_BUFFER_SIZE   512


#define WM_SERIAL_TX_LOOP_DELAY     10
#define WM_SERIAL_TX_TIMEOUT        200
#define WM_SERIAL_TX_NB_TRY         50



#define WM_ACTION_ALL          '$'
#define WM_ACTION_READ         'r'
#define WM_ACTION_WRITE        'w'
#define WM_ACTION_MAP          'm'
#define WM_ACTION_ISNC         'c'
#define WM_ACTION_WRITE_LOCK   'W'
#define WM_ACTION_READ_UNLOCK  'R'
#define WM_ACTION_SAVE         '!'
#define WM_ACTION_RESET        '~'
#define WM_ACTION_SLEEP        '.'



#define WM_PATH_SEPARATOR    '/'
#define WM_DATA_SEPARATOR    ' '
#define WM_CR                '\r'
#define WM_LF                '\n'
#define WM_CRLF              "\r\n"



/** HELP **/
#define WM_TEXT_HELP_F              "** HELP\n** > /action/relay/extra\n** < state relay pin isNc\n\n"
#define WM_CHAR_RX_BEGIN            '\n'
#define WM_CHAR_RX_END              '-'
#define WM_CHAR_RX_UNKNOWN          '?'
#define WM_HEADER_BEGIN_F           "HTTP/1.0 200" WM_CRLF "Content-Type: text/"
#define WM_HEADER_END_ACTION_F      "plain" WM_CRLF WM_CRLF
#define WM_HEADER_END_HELP_GZ_F     "html" WM_CRLF "Content-Encoding: gzip" WM_CRLF WM_CRLF
#define WM_HEADER_END_HELP_BR_F     "html" WM_CRLF "Content-Encoding: br" WM_CRLF WM_CRLF
#define WM_HEADER_WAKEUP_F          "HTTP/1.0 204" WM_CRLF WM_CRLF
 

#endif // constants_H_
