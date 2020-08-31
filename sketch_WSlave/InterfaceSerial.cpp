#include "InterfaceSerial.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void InterfaceSerial::begin()
{
  Serial.begin(USB_SPEED);
}


void InterfaceSerial::loop()
{
  this->setStream(&Serial);

  if (Serial.available() > 0) {
    if (this->read()) {
      this->process();
      Serial.println();
    } else {
      #if MODE_VERBOSE & MODE_VERBOSE_HELP
      Serial.println(TEXT_HELP);
      #endif
    }

    this->terminate();
  }
}


void InterfaceSerial::raise()
{
  LOGLN(F("[raise]"));
}


void InterfaceSerial::reset()
{
  LOGLN(F("[reset]"));
}
