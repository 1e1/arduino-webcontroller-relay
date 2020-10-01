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
  WS_SERIAL.begin(WS_USB_SPEED);

  this->setStream(&WS_SERIAL);
}


void InterfaceSerial::loop()
{
  if (WS_SERIAL.available() > 0) {
    if (this->read()) {
      this->process();
    } else {
      if (Energy.isFirstLoop()) {
        WS_SERIAL.print(WS_CHAR_WAKEUP_F);
      }
    }

    this->terminate();
  }
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




char InterfaceSerial::_read()
{
    uint8_t nbRetry = _READ_NB_RETRY;
    int c;

    do {
        c = WS_SERIAL.read();
        if (c>=0) {
            break;
        }

        delay(1);
    } while (--nbRetry);

    return c;
}
