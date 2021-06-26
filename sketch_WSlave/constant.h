#ifndef constant_H_
#define constant_H_


/* mode */
#define WS_INTERFACE_USB            1
#define WS_INTERFACE_ETHERNET       2
#define WS_INTERFACE_WIFI           4
#define WS_INTERFACE_SERIAL         8
#define WS_INTERFACE_ALL            ( WS_INTERFACE_USB + WS_INTERFACE_ETHERNET )
#define WS_INTERFACE_COUNT(m)       ( bool(m & WS_INTERFACE_USB) + bool(m & WS_INTERFACE_ETHERNET) + bool(m & WS_INTERFACE_WIFI) + bool(m & WS_INTERFACE_SERIAL) )

#define WS_VERBOSE_NONE             0
#define WS_VERBOSE_HELP             1
#define WS_VERBOSE_LIST             2
#define WS_VERBOSE_WEBAPP           4
#define WS_VERBOSE_LENGTH           8
#define WS_VERBOSE_ALL              ( WS_VERBOSE_HELP + WS_VERBOSE_LIST + WS_VERBOSE_WEBAPP + WS_VERBOSE_LENGTH)

#define WS_WIRING_RELAY_NO          0
#define WS_WIRING_RELAY_NC          1

#define WS_MAC_TYPE_STATIC          0
#define WS_MAC_TYPE_DYNAMIC         1

#define WS_BONJOUR_MODE_NONE        0
#define WS_BONJOUR_MODE_STATIC      1
#define WS_BONJOUR_MODE_DYNAMIC     2

#define WS_ACL_ALLOW_NONE           0
#define WS_ACL_ALLOW_RESET          1
#define WS_ACL_ALLOW_SLEEP          2
#define WS_ACL_ALLOW_LOCK           4
#define WS_ACL_ALLOW_ABOUT          8
#define WS_ACL_ALLOW_ALL            ( WS_ACL_ALLOW_RESET + WS_ACL_ALLOW_SLEEP + WS_ACL_ALLOW_LOCK + WS_ACL_ALLOW_ABOUT )

#define WS_STORAGE_NONE             0
#define WS_STORAGE_EEPROM           1

#define WS_LOG_LEVEL_OFF            0
#define WS_LOG_LEVEL_NONE           0
#define WS_LOG_LEVEL_FATAL          10
#define WS_LOG_LEVEL_ERROR          20
#define WS_LOG_LEVEL_WARN           30
#define WS_LOG_LEVEL_INFO           40
#define WS_LOG_LEVEL_DEBUG          50
#define WS_LOG_LEVEL_TRACE          60
#define WS_LOG_LEVEL_ALL            70

#define WS_LOWPOWER_NONE            0
#define WS_LOWPOWER_DOWNCLOCK       1   /* (Timer0,) Timer1, Timer2 */
#define WS_LOWPOWER_NO_ADC          2   /* No Analog */
#define WS_LOWPOWER_NO_SPI          4   /* No Ethernet Shield */
#define WS_LOWPOWER_NO_TWI          8   /* No I2C link */
#define WS_LOWPOWER_NO_USART        16  /* No Serial */
// #define WS_LOWPOWER_NO_INTERRUPT    32  /* noInterrupts() */
#define WS_LOWPOWER_HAS_DELAY       64
#define WS_LOWPOWER_NO_LED          128
#define WS_LOWPOWER_INTERRUPTS      256
#define WS_LOWPOWER_AUTOSLEEP       512
#define WS_LOWPOWER_ALL             ( WS_LOWPOWER_DOWNCLOCK + WS_LOWPOWER_NO_ADC + WS_LOWPOWER_NO_SPI + WS_LOWPOWER_NO_TWI + WS_LOWPOWER_NO_USART + WS_LOWPOWER_HAS_DELAY + WS_LOWPOWER_NO_LED + WS_LOWPOWER_INTERRUPTS + WS_LOWPOWER_AUTOSLEEP )
#define WS_LOWPOWER_WAKEUP_BOD_MS   65
#define WS_WAKEUP_PCINT_SERIAL0     PCINT8
#define WS_WAKEUP_PCINT_SERIAL1     INT2
#define WS_WAKEUP_PCINT_SERIAL3     PCINT9
#define WS_SLEEP_TIME_NONE          T_NONE
#define WS_SLEEP_TIME_MIN           T_16MS
#define WS_SLEEP_TIME_MAX           T_8S
#define WS_SLEEP_TIME_INFINITE      T_INFINITE

#define WS_HTML_COMPRESSION_GZ      0
#define WS_HTML_COMPRESSION_BR      1



#define WS_ACTION_ALL          '$'
#define WS_ACTION_LENGTH       '#'
#define WS_ACTION_READ         'r'
#define WS_ACTION_WRITE        'w'
#define WS_ACTION_MAP          'm'
#define WS_ACTION_ISNC         'c'
#define WS_ACTION_WRITE_LOCK   'W'
#define WS_ACTION_READ_UNLOCK  'R'
#define WS_ACTION_SAVE         '!'
#define WS_ACTION_RESET        '~'
#define WS_ACTION_SLEEP        '.'
#define WS_ACTION_ABOUT        '?'



#define WS_PATH_SEPARATOR    '/'
#define WS_DATA_SEPARATOR    ' '
#define WS_CR                '\r'
#define WS_LF                '\n'
#define WS_CRLF              "\r\n"



/** HELP **/
#define WS_TEXT_HELP_F              "** HELP\n** > /action/relay/extra\n** < state relay locked isNc pin\n\n"
#define WS_CHAR_RX_UNKNOWN          '?'
#define WS_CHAR_TX_BEGIN            '\n'
#define WS_CHAR_TX_END              '-'
#define WS_HEADER_BEGIN_F           "HTTP/1.0 200" WS_CRLF "Content-Type: text/"
#define WS_HEADER_END_ACTION_F      "plain" WS_CRLF WS_CRLF
#define WM_HEADER_END_HELP_GZ_F     "html" WS_CRLF "Content-Encoding: gzip" WS_CRLF WS_CRLF
#define WM_HEADER_END_HELP_BR_F     "html" WS_CRLF "Content-Encoding: br" WS_CRLF WS_CRLF
#define WS_HEADER_WAKEUP_F          "HTTP/1.0 204" WS_CRLF WS_CRLF
 

#endif // constant_H_
