#include "Relay.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




Relay::option Relay::_options[NB_RELAYS];





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/



const bool Relay::exists(const uint8_t relayId)
{
    return Relay::getPinAt(relayId) != PIN_NONE;
}


const uint8_t Relay::getPinAt(const uint8_t relayId)
{
    return Relay::_options[relayId].pin;
}


void Relay::setPinAt(const uint8_t relayId, const uint8_t pin)
{
    Relay::_options[relayId].pin = pin;
}


const bool Relay::isNcAt(const uint8_t relayId)
{
    return Relay::_options[relayId].isNc;
}


void Relay::isNcAt(const uint8_t relayId, const bool isNc)
{
    Relay::_options[relayId].isNc = isNc;
}


const bool Relay::getStateAt(const uint8_t relayId)
{
    return Relay::_options[relayId].isNc ^ Relay::_options[relayId].value;
}


void Relay::setStateAt(const uint8_t relayId, const bool state)
{
    Relay::_options[relayId].value = Relay::_options[relayId].isNc ^ state;
}




/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/
