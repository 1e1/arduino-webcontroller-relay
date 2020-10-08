#include "Relay.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




uint8_t Relay::_options[Relay::optionsLength];

static const uint8_t _MASK_PIN       = B00111111;
static const uint8_t _BIT_IS_NC      = 6;
static const uint8_t _BIT_IS_LOCK    = 7;
//static const uint8_t _BIT_IS_ACTIVE  = 7;
static const uint8_t _PIN_NONE       = -1;
static const uint8_t _PIN_MAX        = _MASK_PIN & _PIN_NONE;
static const byte    _EEPROM_VOID    = -1;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/



void Relay::begin()
{
  #if WS_STORAGE & WS_STORAGE_EEPROM
  uint8_t option;
  for (int i=0; i<Relay::optionsLength && i<EEPROM.length(); ++i) {
    option = EEPROM.read(i);

    if (_EEPROM_VOID != option) {
        Relay::_options[i] = option;

        Relay::_init(i);
    }
  }
  #endif
}


void Relay::save()
{
  #if WS_STORAGE & WS_STORAGE_EEPROM
  for (int i=0; i<Relay::optionsLength && i<EEPROM.length(); ++i) {
    Relay::_save(i);
  }
  #endif
}


const uint8_t Relay::getPinAt(const uint8_t relayId)
{
    return Relay::_options[relayId] & _MASK_PIN;
}


void Relay::setPinAt(const uint8_t relayId, const uint8_t pin)
{
    Relay::_options[relayId] = (Relay::_options[relayId] & ~_MASK_PIN) | (pin & _MASK_PIN);

    Relay::_init(relayId);
}


const bool Relay::isNcAt(const uint8_t relayId)
{
    return bitRead_boolean(Relay::_options[relayId], _BIT_IS_NC);
}


void Relay::isNcAt(const uint8_t relayId, const bool isNc)
{
    bitWrite_boolean(Relay::_options[relayId], _BIT_IS_NC, isNc);
}


const bool Relay::isLocked(const uint8_t relayId)
{
    return bitRead_boolean(Relay::_options[relayId], _BIT_IS_LOCK);
}


void Relay::isLocked(const uint8_t relayId, const bool isLocked)
{
    bitWrite_boolean(Relay::_options[relayId], _BIT_IS_LOCK, isLocked);
}


const bool Relay::getStateAt(const uint8_t relayId)
{
    const uint8_t pinId = Relay::getPinAt(relayId);

    return Relay::isNcAt(relayId) ^ digitalRead(pinId);
}


void Relay::setStateAt(const uint8_t relayId, const bool state, const bool force)
{
    if (Relay::isLocked(relayId) <= force) {
        const uint8_t pin = Relay::getPinAt(relayId);
        const bool isActive = Relay::isNcAt(relayId) ^ state;

        digitalWrite(pin, isActive ? HIGH : LOW);
    }
}




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/



void Relay::_init(const uint8_t relayId)
{
    if (Relay::_exists(relayId)) {
        const uint8_t pin = Relay::getPinAt(relayId);

        pinMode(pin, OUTPUT);
    }
}


const bool Relay::_exists(const uint8_t relayId)
{
    return Relay::getPinAt(relayId) != _PIN_NONE & _MASK_PIN;
}


void Relay::_save(const uint8_t relayId)
{
    #if WS_STORAGE & WS_STORAGE_EEPROM
    const uint8_t data = Relay::_options[relayId] & (_MASK_PIN | (B1 << _BIT_IS_NC));

    EEPROM.update(relayId, data);
    #endif
}
