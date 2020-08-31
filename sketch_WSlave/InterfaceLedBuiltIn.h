#ifndef InterfaceLedBuiltIn_H_
#define InterfaceLedBuiltIn_H_

#include <Arduino.h>
#include "AbstractInterface.h"



class InterfaceLedBuiltIn : public AbstractInterface {

  public:
  typedef enum { OFF=0, IDLE=15, WORK=63, ON=255 } State;

  #ifdef LED_BUILTIN
  InterfaceLedBuiltIn():InterfaceLedBuiltIn(LED_BUILTIN) {};
  #endif
  InterfaceLedBuiltIn(const byte pin);
  __attribute__((always_inline)) inline void state(const State value) { analogWrite(this->_pin, value); };

  inline void loop()  { this->state(State::OFF ); };
  inline void raise() { this->state(State::IDLE); };
  inline void reset() { this->state(State::WORK); };

  protected:
  byte _pin;

};


#endif
