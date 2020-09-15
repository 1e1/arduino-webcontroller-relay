#ifndef constants_H_
#define constants_H_


/* mode */
#define WS_INTERFACE_USB            1
#define WS_INTERFACE_ETHERNET       2
#define WS_INTERFACE_WIFI           4
#define WS_INTERFACE_ALL            ( WS_INTERFACE_USB + WS_INTERFACE_ETHERNET )
#define WS_INTERFACE_COUNT(m)       ( bool(m & WS_INTERFACE_USB) + bool(m & WS_INTERFACE_ETHERNET) + bool(m & WS_INTERFACE_WIFI) )

#define WS_VERBOSE_NONE             0
#define WS_VERBOSE_HELP             1
#define WS_VERBOSE_LIST             2
#define WS_VERBOSE_WEBAPP           4
#define WS_VERBOSE_ALL              (WS_VERBOSE_HELP + WS_VERBOSE_LIST + WS_VERBOSE_WEBAPP)

#define WS_WIRING_RELAY_NO          0
#define WS_WIRING_RELAY_NC          1

#define WS_MAC_TYPE_STATIC          0
#define WS_MAC_TYPE_DYNAMIC         1

#define WS_BONJOUR_MODE_NONE        0
#define WS_BONJOUR_MODE_STATIC      1
#define WS_BONJOUR_MODE_DYNAMIC     2

#define WS_ACL_ALLOW_NONE           0
#define WS_ACL_ALLOW_RESET          1

#define WS_STORAGE_NONE             0
#define WS_STORAGE_EEPROM           1

#define WS_LOG_LEVEL_OFF            0
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
#define WS_LOWPOWER_NO_INTERRUPT    32  /* noInterrupts() */
#define WS_LOWPOWER_HAS_DELAY       64  
#define WS_LOWPOWER_ALL             ( WS_LOWPOWER_DOWNCLOCK + WS_LOWPOWER_NO_ADC + WS_LOWPOWER_NO_SPI + WS_LOWPOWER_NO_TWI + WS_LOWPOWER_NO_USART + WS_LOWPOWER_NO_INTERRUPT + WS_LOWPOWER_HAS_DELAY )
#define WS_LOWPOWER_DELAY_MS        250
#define WS_LOWPOWER_CLOCK_DIVISOR   8



#define WS_ACTION_ALL          '$'
#define WS_ACTION_READ         'r'
#define WS_ACTION_WRITE        'w'
#define WS_ACTION_MAP          'm'
#define WS_ACTION_NC           'c'
#define WS_ACTION_NO           'o'
#define WS_ACTION_SAVE         '!'
#define WS_ACTION_RESET        '~'



#define WS_PATH_SEPARATOR    '/'
#define WS_DATA_SEPARATOR    ' '
#define WS_CR                '\r'
#define WS_LF                '\n'
#define WS_CRLF              "\r\n"



/** HELP **/
#define WS_TEXT_HELP_F         "** HELP\n** > /action/relay/extra\n** < state relay pin isNc\n\n"
#define WS_HEADER_BEGIN_F      "HTTP/1.0 200" WS_CRLF "Content-Type: text/"
#define WS_HEADER_END_ACTION_F "plain" WS_CRLF WS_CRLF
#define WS_HEADER_END_HELP_F   "html" WS_CRLF "Content-Encoding: gzip" WS_CRLF WS_CRLF
 

#endif // constants_H_
