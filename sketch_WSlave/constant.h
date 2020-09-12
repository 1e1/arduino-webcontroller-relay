#ifndef constants_H_
#define constants_H_


/* mode */
#define WS_MODE_SERIAL_USB         1
#define WS_MODE_SERIAL_ETHERNET    2
#define WS_MODE_SERIAL_LED         4
#define WS_MODE_SERIAL_ALL         (WS_MODE_SERIAL_USB + WS_MODE_SERIAL_ETHERNET + WS_MODE_SERIAL_LED)
#define WS_MODE_SERIAL_COUNT(m)    (bool(m & WS_MODE_SERIAL_USB) + bool(m & WS_MODE_SERIAL_ETHERNET))
#define WS_MODE_VERBOSE_NONE       0
#define WS_MODE_VERBOSE_HELP       1
#define WS_MODE_VERBOSE_LIST       2
#define WS_MODE_VERBOSE_WEBAPP     4
#define WS_MODE_VERBOSE_ALL        (WS_MODE_VERBOSE_HELP + WS_MODE_VERBOSE_LIST + WS_MODE_VERBOSE_WEBAPP)
#define WS_RELAY_WIRING_NO         0
#define WS_RELAY_WIRING_NC         1
#define WS_TYPE_MAC_STATIC         0
#define WS_TYPE_MAC_DYNAMIC        1
#define WS_MODE_BONJOUR_NONE       0
#define WS_MODE_BONJOUR_STATIC     1
#define WS_MODE_BONJOUR_DYNAMIC    2
#define WS_ACL_ALLOW_NONE          0
#define WS_ACL_ALLOW_RESET         1
#define WS_DATA_STORAGE_NONE       0
#define WS_DATA_STORAGE_EEPROM     1
#define WS_LOG_LEVEL_OFF           0
#define WS_LOG_LEVEL_FATAL         10
#define WS_LOG_LEVEL_ERROR         20
#define WS_LOG_LEVEL_WARN          30
#define WS_LOG_LEVEL_INFO          40
#define WS_LOG_LEVEL_DEBUG         50
#define WS_LOG_LEVEL_TRACE         60
#define WS_LOG_LEVEL_ALL           70



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
#define WS_TEXT_HELP_P         "** HELP\n** > /action/relay/extra\n** < state relay pin isNc\n\n"
#define WS_HEADER_BEGIN_P      "HTTP/1.0 200" WS_CRLF "Content-Type: text/"
#define WS_HEADER_END_ACTION_P "plain" WS_CRLF WS_CRLF
#define WS_HEADER_END_HELP_P   "html" WS_CRLF "Content-Encoding: gzip" WS_CRLF WS_CRLF
 

#endif // constants_H_
