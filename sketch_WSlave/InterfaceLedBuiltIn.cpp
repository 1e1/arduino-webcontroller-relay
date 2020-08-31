#include "InterfaceLedBuiltIn.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




//




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceLedBuiltIn::InterfaceLedBuiltIn(const byte pin)
{
  this->_pin = pin;
  pinMode(this->_pin, OUTPUT);
}
