#ifndef LedBuiltIn_H_
#define LedBuiltIn_H_

#include <Arduino.h>
#include "AbstractInterface.h"



class LedBuiltIn : public AbstractInterface {

  public:
  typedef enum { OFF=0, IDLE=15, WORK=63, ON=255 } State;

  LedBuiltIn(const byte pin);
  __attribute__((always_inline)) inline void state(const State value) { analogWrite(this->_pin, value); };

  inline void loop()  { this->state(State::ON  ); };
  inline void raise() { this->state(State::WORK); };
  inline void reset() { this->state(State::IDLE); };
  inline void clean() { this->state(State::OFF ); };

  protected:
  byte _pin;

};


#endif
