#include "InterfaceUsb.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




static const uint8_t _READ_NB_RETRY = 128;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void InterfaceUsb::begin()
{
  Serial.begin(WS_USB_SPEED);
  
  this->setStream(&Serial);
}


void InterfaceUsb::loop()
{
  if (Serial.available() > 0) {
    if (this->read()) {
      this->process();
      LOGLN();
    } else {
      #if WS_VERBOSE & WS_VERBOSE_HELP
      Serial.print(F(WS_TEXT_HELP_F));
      #endif
    }

    this->terminate();
  }
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




const char InterfaceUsb::_read() const
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
