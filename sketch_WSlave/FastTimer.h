#ifndef FastTimer_H_
#define FastTimer_H_

#include <Arduino.h>


/** TIME **/
//==>  >> 0: 1 unit of embedTime is 0.001s
//-->  const unsigned long maxTime        = 4294967295; // = 49d 17h 02m 47s
//-->  const unsigned int maxTime         = 65535;      // = 65s
//==>  >> 10: 1 unit of embedTime is 1.024s
//-->  const uint8_t maxEmbedTime         = 255;        // = 4mn 21s 120ms
//==>  >> 12: 1 unit of embedTime is 4.096s
//-->  const unsigned int maxEmbedTime    = 65535;      // = 3d 02h 33mn 51s 360ms
//     const unsigned int moduloEmbedTime = 63281;      // = 3d 00h 00mn 00s 000ms
//-->  const uint8_t maxEmbedTime         = 255;        // = 17mn 24s 480ms
//     const uint8_t moduloEmbedTime      = 219;        // = 15mn 00s 000ms
//==>  >> 14: 1 unit of embedTime is 16.384s
//-->  const uint8_t maxEmbedTime         = 255;        // = 1h 09mn 37s 920ms
//     const uint8_t moduloEmbedTime      = 219;        // = 1h 00mn 00s 000ms
//==>  >> 16: 1 unit of embedTime is 65.536s
//-->  const uint8_t maxEmbedTime         = 255;        // = 4h 38mn 31s 680ms
//     const uint8_t moduloEmbedTime      = 219;        // = 4h 00mn 00s 000ms

#define WS_FASTTIMER_PRECISION_1s_4m    byte(millis() >> 10)
#define WS_FASTTIMER_PRECISION_4s_15m   byte(millis() >> 12)
//#define WS_EMBEDTIME_4s_3d    int(millis() >> 12)
#define WS_FASTTIMER_PRECISION_16s_1h   byte(millis() >> 14)
#define WS_FASTTIMER_PRECISION_65s_4h   byte(millis() >> 16)


#define WS_FASTTIMER_SECTION_1   7
#define WS_FASTTIMER_SECTION_2   6
#define WS_FASTTIMER_SECTION_4   5
#define WS_FASTTIMER_SECTION_8   4
#define WS_FASTTIMER_SECTION_16  3
#define WS_FASTTIMER_SECTION_32  2
#define WS_FASTTIMER_SECTION_64  1
#define WS_FASTTIMER_SECTION_128 0


/** BIT **/
#define bitRead_boolean(value, bit)             byte(((value) >> (bit)) & byte(1))
/** === **/


class FastTimer {

  public:
  typedef enum { P1s_4m=10, P4s_15m=12, P16s_1h=14, P65s_4h=16 } Precision;

  FastTimer(const Precision precision);

  void begin(); // call it in setup()
  const uint8_t update(); // call it once in the main loop()

  // inline
  __attribute__((always_inline)) inline const boolean isSection(const byte section) { return bitRead_boolean(this->_section, section); };
  __attribute__((always_inline)) inline const uint8_t getTime() { return this->_embedTime; };

  protected:
  Precision _precision;
  uint8_t _section;
  uint8_t _embedTime;

};


#endif
