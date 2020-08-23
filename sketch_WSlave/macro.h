#ifndef macro_H_
#define macro_H_


/* mode */
#define MODE_SERIAL_USB         1
#define MODE_SERIAL_ETHERNET    2
#define MODE_SERIAL_ALL         (MODE_SERIAL_USB + MODE_SERIAL_ETHERNET)
#define MODE_SERIAL_COUNT(m)    (bool(m & MODE_SERIAL_USB) + bool(m & MODE_SERIAL_ETHERNET))
#define MODE_VERBOSE_NONE       0
#define MODE_VERBOSE_HELP       1
#define MODE_VERBOSE_LIST       2
#define MODE_VERBOSE_WEBAPP     4
#define MODE_VERBOSE_BONJOUR    8
#define MODE_VERBOSE_ALL        (MODE_VERBOSE_HELP + MODE_VERBOSE_LIST + MODE_VERBOSE_WEBAPP + MODE_VERBOSE_BONJOUR)
#define RELAY_WIRING_NO         0
#define RELAY_WIRING_NC         1
#define TYPE_MAC_STATIC         0
#define TYPE_MAC_RANDOM         1
#define ACL_ALLOW_NONE          0
#define ACL_ALLOW_RESET         1
#define DATA_STORAGE_NONE       0
#define DATA_STORAGE_EEPROM     1


/** HELP **/
#define TEXT_HELP F("** HELP\n** /<action>\\w/(<relay>\\d+(/<extra>\\d+)?)?\n")


#define DEBUG 0


/** DEBUGGING TOOLS **/
#if DEBUG
  #define DEBUG_START() Serial.begin(USB_SPEED)
  #define LOG(...)      Serial.print  (__VA_ARGS__)
  #define LOGLN(...)    Serial.println(__VA_ARGS__)
  #define WAIT(ms)      delay(ms)
#else
  #define DEBUG_START()
  #define LOG(...)
  #define LOGLN(...)
  #define WAIT(ms)
#endif
/** === **/



#ifdef LED_BUILTIN
  #define BUSYLED_HIGH analogWrite(LED_BUILTIN, 255)
  #define BUSYLED_WORK analogWrite(LED_BUILTIN, 63)
  #define BUSYLED_IDLE analogWrite(LED_BUILTIN, 15)
  #define BUSYLED_NONE analogWrite(LED_BUILTIN, 0)
#else
  #define BUSYLED_HIGH
  #define BUSYLED_WORK
  #define BUSYLED_IDLE
  #define BUSYLED_NONE
#endif



/** SETUP ALIAS **/
#define LCDCHAR(ch)                     static const uint8_t ch[] PROGMEM
#define LONGBYTES(b)                    static const unsigned char b[] PROGMEM
#define LONGBYTEN(b, n)                 static const unsigned char b[n] PROGMEM
#define LONGSTRING(str)                 static const char str[] PROGMEM
/** === **/



/** NETWORK **/
#define MAC_ADDRESS(index)      { 0x40,0x6C,0x61,0x6E,0x23, index }
#define DEVICE_NAME_MDNS(dn)    dn "_##"



/** FAST TRIGO **/
#define FACTOR_PI_UINT12(x) byte((50*x) >> 4)
#define FACTOR_PI_UINT8(x)  byte((804*x) >> 8)
/** === **/



/** LOOP **/
// wikipedia "foreach_loop"
#define FOREACH(idxtype, idxpvar, col, colsiz ) idxtype* idxpvar; for( idxpvar=col ; idxpvar < (col + (colsiz)) ; idxpvar++)
#define ARRAYLEN( ary ) ( sizeof(ary)/sizeof(*ary) )
#define FILLARRAY(a,n) a[0]=n, memcpy( ((char*)a)+sizeof(a[0]), a, sizeof(a)-sizeof(a[0]) );
#define FAST_STRCMP(x, y)  (*(x) != *(y) ? \
    ((unsigned char) *(x) - (unsigned char) *(y)) : \
    strcmp((x), (y)))
#define FAST_STRCMP_P(x, y)  (*(x) != *(y) ? \
    ((unsigned char) *(x) - (unsigned char) *(y)) : \
    strcmp_P((x), (y)))
/*
FOREACH (char, p1, c1, strlen(c1) ) {
  printf("loop 1 : %c\n",*p1);
}
FOREACH (int, p2, c2, ARRAYLEN(c2) ){
  printf("loop 2 : %d\n",*p2);
}
*/
/** === **/



/** BIT **/
#define bitRead_uint8_t(value, bit)             byte(((value) >> (bit)) & byte(1))
#define bitSet_uint8_t(value, bit)              ((value) |=  (byte(1) << (bit)))
#define bitClear_uint8_t(value, bit)            ((value) &= ~(byte(1) << (bit)))
#define bitWrite_uint8_t(value, bit, bitvalue)  (bitvalue ? bitSet_uint8_t(value, bit) : bitClear_uint8_t(value, bit))

#define bitRead_boolean(value, bit)             byte(((value) >> (bit)) & byte(1))
#define bitSet_boolean(value, bit)              ((value) |=  (byte(1) << (bit)))
#define bitClear_boolean(value, bit)            ((value) &= ~(byte(1) << (bit)))
#define bitWrite_boolean(value, bit, bitvalue)  (bitvalue ? bitSet_boolean(value, bit) : bitClear_boolean(value, bit))
/** === **/



#define STR_HELPER(x) #x
#define STR(x) STR_HELPER(x)


#define SOFTWARE_RESET    asm volatile ("jmp 0");


#endif // macro_H_
