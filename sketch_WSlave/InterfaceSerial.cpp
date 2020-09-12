#include "InterfaceSerial.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




static const uint8_t _READ_NB_RETRY = 128;





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
      LOGLN();
    } else {
      #if WS_MODE_VERBOSE & WS_MODE_VERBOSE_HELP
      Serial.print(F(WS_TEXT_HELP_P));
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



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




char InterfaceSerial::_read()
{
    uint8_t nbRetry = _READ_NB_RETRY;
    int c;

    do {
        c = Serial.read();
        if (c>=0) {
            break;
        }

        delay(1);
    } while (--nbRetry);

    return c;
}
