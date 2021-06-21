#ifndef InterfaceSerial_H_
#define InterfaceSerial_H_

#include <Arduino.h>
#include "config.h"
#include "macro.h"
#include "AbstractStream.h"
#include "PowerManager.h"



class InterfaceSerial : public AbstractStream {

  public:
  void begin(void);
  void loop(void);

  protected:
  const char _read(void) const;

};


#endif
