#include "Relay.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




uint8_t Relay::_options[NB_RELAYS];

uint8_t _MASK_PIN       = B00111111;
uint8_t _BIT_IS_NC      = 6;
uint8_t _BIT_IS_ACTIVE  = 7;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/



void Relay::begin()
{
  #if STORAGE & STORAGE_EEPROM
  uint8_t option;
  for (int i=0; i<NB_RELAYS && i<EEPROM.length(); i++) {
    option = EEPROM.read(i);

    if (EEPROM_VOID != option) {
        Relay::_options[i] = option;
    }
  }
  #endif
}


const bool Relay::exists(const uint8_t relayId)
{
    return Relay::getPinAt(relayId) != PIN_NONE;
}


void Relay::save()
{
  #if STORAGE & STORAGE_EEPROM
  for (int i=0; i<NB_RELAYS && i<EEPROM.length(); i++) {
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
    return bitRead_boolean(Relay::_options[relayId], _BIT_IS_NC) ^ bitRead_boolean(Relay::_options[relayId], _BIT_IS_ACTIVE);
}


void Relay::setStateAt(const uint8_t relayId, const bool state)
{
    bitWrite_boolean(
            Relay::_options[relayId],
            _BIT_IS_ACTIVE,
            bitRead_boolean(Relay::_options[relayId], _BIT_IS_NC) ^ state
    );
}




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/



void Relay::_save(const uint8_t relayId)
{
    #if STORAGE & STORAGE_EEPROM
    const uint8_t data = Relay::_options[relayId] & (_MASK_PIN | (B1 << _BIT_IS_NC));

    EEPROM.update(relayId, data);
    #endif
}
