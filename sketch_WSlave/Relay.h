#ifndef Core_h
#define Core_h



#include <Arduino.h>
#include <EEPROM.h>
#include "config.h"
#include "macro.h"



class Relay {

  public:
  static const uint8_t optionsLength = min(NB_RELAYS_MAX, NUM_DIGITAL_PINS);

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
  static void _init(const uint8_t relayId);
  static bool _isActive(const uint8_t relayId);
  static void _digitalWrite(const uint8_t relayId);
  static void _save(const uint8_t relayId);

  static uint8_t _options[optionsLength];

};



#endif
