#include "Relay.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




uint8_t Relay::_options[Relay::optionsLength];

static const uint8_t _MASK_PIN       = B00111111;
static const uint8_t _BIT_IS_NC      = 6;
static const uint8_t _BIT_IS_ACTIVE  = 7;
static const uint8_t _PIN_NONE       = -1;
static const byte    _EEPROM_VOID    = -1;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/



void Relay::begin()
{
  #if WS_STORAGE & WS_STORAGE_EEPROM
  uint8_t option;
  for (int i=0; i<Relay::optionsLength && i<EEPROM.length(); i++) {
    option = EEPROM.read(i);

    if (_EEPROM_VOID != option) {
        Relay::_options[i] = option;

        Relay::_init(i);
    }
  }
  #endif
}


const bool Relay::exists(const uint8_t relayId)
{
    return Relay::getPinAt(relayId) != _PIN_NONE & _MASK_PIN;
}


void Relay::save()
{
  #if WS_STORAGE & WS_STORAGE_EEPROM
  for (int i=0; i<Relay::optionsLength && i<EEPROM.length(); i++) {
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


const bool Relay::getStateAt(const uint8_t relayId)
{
    return Relay::isNcAt(relayId) ^ Relay::_isActive(relayId);
}


void Relay::setStateAt(const uint8_t relayId, const bool state)
{
    bitWrite_boolean(
        Relay::_options[relayId],
        _BIT_IS_ACTIVE,
        Relay::isNcAt(relayId) ^ state
    );

    Relay::_digitalWrite(relayId);
}




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/



void Relay::_init(const uint8_t relayId)
{
    if (Relay::exists(relayId)) {
        const uint8_t pin = Relay::getPinAt(relayId);

        pinMode(pin, OUTPUT);
    }
}


bool Relay::_isActive(const uint8_t relayId)
{
    return bitRead_boolean(Relay::_options[relayId], _BIT_IS_ACTIVE);
}


void Relay::_digitalWrite(const uint8_t relayId)
{
    const uint8_t pin = Relay::getPinAt(relayId);
    const bool isActive = Relay::_isActive(relayId);

    digitalWrite(pin, isActive ? HIGH : LOW);
}


void Relay::_save(const uint8_t relayId)
{
    #if WS_STORAGE & WS_STORAGE_EEPROM
    const uint8_t data = Relay::_options[relayId] & (_MASK_PIN | (B1 << _BIT_IS_NC));

    EEPROM.update(relayId, data);
    #endif
}
