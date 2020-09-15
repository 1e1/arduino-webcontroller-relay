#ifndef PowerManager_H_
#define PowerManager_H_

#include <Arduino.h>
#include <avr/power.h>
#include "macro.h"

  // clock_div_1    =>  16 MHz
  // clock_div_2    =>   8 MHz
  // clock_div_4    =>   4 MHz
  // clock_div_8    =>   2 MHz
  // clock_div_16   =>   1 MHz
  // clock_div_32   => 500  Hz
  // clock_div_64   => 250  Hz
  // clock_div_128  => 125  Hz


class PowerManager {

  public:
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

  __attribute__((always_inline)) inline static void free() { power_all_enable(); };

  PowerManager(const Frequency frequency);

  void begin();

  const uint8_t update(); // call it once in the main loop()

};


#endif
