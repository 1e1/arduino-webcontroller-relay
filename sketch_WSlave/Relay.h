#ifndef Core_h
#define Core_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macro.h"



#define PIN_NONE    ((uint8_t) -1)



class Relay {

  public:
  static const bool exists(const uint8_t relayId);

  static const uint8_t getPinAt(const uint8_t relayId);
  static void setPinAt(const uint8_t relayId, const uint8_t pin);

  static const bool isNcAt(const uint8_t relayId);
  static void isNcAt(const uint8_t relayId, const bool isNc);

  static const bool getStateAt(const uint8_t relayId);
  static void setStateAt(const uint8_t relayId, const bool value);

  protected:
  struct option {
    option(): pin(PIN_NONE), isNc(RELAY_WIRING), value(false) {};
    bool isNc : 1;
    bool value : 1;
    uint8_t pin;
  };

  static option _options[NB_RELAYS];

};



#endif
