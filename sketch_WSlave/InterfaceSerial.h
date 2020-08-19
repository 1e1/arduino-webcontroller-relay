#ifndef InterfaceSerial_H_
#define InterfaceSerial_H_

#include <Arduino.h>
#include "config.h"
#include "macro.h"
#include "AbstractStream.h"
#include "Relay.h"



class InterfaceSerial : public AbstractStream {

  public:
  void begin(); // call it in setup()
  void loop();  // call it once in the main loop()
  void raise();
  void reset();

};


#endif
