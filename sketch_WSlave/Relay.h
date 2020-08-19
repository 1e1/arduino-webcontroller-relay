#ifndef Core_h
#define Core_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include <EEPROM.h>
#include "config.h"
#include "macro.h"



#define PIN_NONE      ((uint8_t) -1)
#define EEPROM_VOID   ((byte) -1)



class Relay {

  public:
  static void begin();
  static const bool exists(const uint8_t relayId);
  static void save();

  static const uint8_t getPinAt(const uint8_t relayId);
  static void setPinAt(const uint8_t relayId, const uint8_t pin);

  static const bool isNcAt(const uint8_t relayId);
  static void isNcAt(const uint8_t relayId, const bool isNc);

  static const bool getStateAt(const uint8_t relayId);
  static void setStateAt(const uint8_t relayId, const bool value);

  protected:
  static void _save(const uint8_t relayId);

  static uint8_t _options[NB_RELAYS];

};



#endif
