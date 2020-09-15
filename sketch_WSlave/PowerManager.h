#ifndef PowerManager_H_
#define PowerManager_H_

#include <Arduino.h>
#include <avr/power.h>
#include <avr/sleep.h>
#include <avr/wdt.h>
#include "macro.h"


class PowerManager {

  public:
  typedef enum { INT_TIMER, INT_EXTERNAL } Interrupt;

  typedef enum { 
    F_16MHz=clock_div_1, 
    F_8MHz=clock_div_2, 
    F_4MHz=clock_div_4, 
    F_2MHz=clock_div_8, 
    F_1MHz=clock_div_16, 
    F_500Hz=clock_div_32, 
    F_250Hz=clock_div_64, 
    F_125Hz=clock_div_128,
  } Frequency;

  typedef enum { 
    T_NONE=0, 
    T_16MS=bit(WDIE), 
    T_32MS=bit(WDIE)|bit(WDP0), 
    T_64MS=bit(WDIE)|bit(WDP1), 
    T_125MS=bit(WDIE)|bit(WDP1)|bit(WDP0), 
    T_250MS=bit(WDIE)|bit(WDP2), 
    T_500MS=bit(WDIE)|bit(WDP2)|bit(WDP0), 
    T_1S=bit(WDIE)|bit(WDP2)|bit(WDP1), 
    T_2S=bit(WDIE)|bit(WDP2)|bit(WDP1)|bit(WDP0), 
    T_4S=bit(WDIE)|bit(WDP3), 
    T_8S=bit(WDIE)|bit(WDP3)|bit(WDP1),
    T_INFINITE=-1, 
  } Wdt;

  __attribute__((always_inline)) inline static void free() { power_all_enable(); };

  static void begin();
  static void loop();
  static const bool isFirstLoop();
  static void highCpu();
  static void lowCpu();
  static void sleep();
  static void hold();

  protected:
  static void _disableComponents();
  static void _enableComponents();
  static void _sleepTimer(const byte mode=SLEEP_MODE_IDLE, const Wdt timer=Wdt::WS_IDLE_TIME);
  static void _sleepUntilInterrupt(const byte mode=SLEEP_MODE_PWR_DOWN);

  static uint8_t _cycleCounter;

};

extern PowerManager Energy;


#endif
