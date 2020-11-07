#ifndef constants_H_
#define constants_H_


/* mode */
#define WM_LOG_LEVEL_OFF            0
#define WM_LOG_LEVEL_FATAL          10
#define WM_LOG_LEVEL_ERROR          20
#define WM_LOG_LEVEL_WARN           30
#define WM_LOG_LEVEL_INFO           40
#define WM_LOG_LEVEL_DEBUG          50
#define WM_LOG_LEVEL_TRACE          60
#define WM_LOG_LEVEL_ALL            70



#define WM_CONFIG_KEY_PATH          "_KEY.txt"
#define WM_CONFIG_CERT_PATH         "_CSR.txt"
#define WM_CONFIG_GLOBAL_PATH       "global.json"
#define WM_CONFIG_WIFI_PATH         "wifis.json"
#define WM_CONFIG_RELAY_PATH        "relays.json"
#define WM_WEB_INDEX_BASENAME       "index"
#define WM_WEB_PORTAL_BASENAME      "portal"

#define WM_WEB_PORT_DEFAULT         80
#define WM_WEB_PORT_DEFAULT_SECURE  443
#define WM_WEB_SERVER_SECURE_NO     0
#define WM_WEB_SERVER_SECURE_YES    1
#define WM_WEB_FILE_EXT_BR          "br"
#define WM_WEB_FILE_EXT_GZ          "gz"

#define WM_CONFIG_BUFFER_SIZE       512


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
#define WM_CHAR_RX_BEGIN            '\n'
#define WM_CHAR_RX_END              '-'
#define WM_CHAR_RX_UNKNOWN          '?'
 

#endif // constants_H_
