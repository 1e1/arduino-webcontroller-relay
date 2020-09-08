#include "InterfaceSerial.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




static uint8_t readLimitNbRetry = 128;





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
      #if MODE_VERBOSE & MODE_VERBOSE_HELP
      Serial.print(TEXT_HELP);
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
    uint8_t nbRetry = readLimitNbRetry;
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
