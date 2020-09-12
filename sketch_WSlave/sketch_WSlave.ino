/*
 Rest Server

 https://github.com/1e1/WSlave

 Circuit at least one:
 * Ethernet shield
 * LCD shield

 created 29 Aug 2012
 by Aymeric GERLIER
 modified 4 Sep 2012
 by Aymeric GERLIER

 */



#include <Arduino.h>
#include "config.h"
#include "macro.h"
#include "InterfaceEthernet.h"
#include "InterfaceSerial.h"
#include "FastTimer.h"
#include "InterfaceLedBuiltIn.h"



/** ===================== **/



static FastTimer* _timer;
static AbstractInterface* _engines[WS_MODE_SERIAL_COUNT(WS_MODE_SERIAL)];




void setup()
{
  DEBUG_START();
  LOGLN(F("DEBUG ON"));

  const FastTimer::Precision p = FastTimer::Precision::P65s_4h;
  _timer = new FastTimer(p);

  Relay::begin();

  uint8_t i = ARRAYLEN(_engines);

  #if WS_MODE_SERIAL & WS_MODE_SERIAL_ETHERNET
  --i;
  _engines[i] = new InterfaceEthernet();
  _engines[i]->begin();
  #endif

  #if WS_MODE_SERIAL & WS_MODE_SERIAL_USB
  --i;
  _engines[i] = new InterfaceSerial();
  _engines[i]->begin();
  #endif

  #if WS_MODE_SERIAL & WS_MODE_SERIAL_LED
  --i;
  _engines[i] = new InterfaceLedBuiltIn();
  _engines[i]->begin();
  #endif

  LOGLN(F("CONFIGURED"));
}


void loop()
{
  const bool isTick = _timer->update();

  uint8_t i = WS_MODE_SERIAL_COUNT(WS_MODE_SERIAL);

  while (i-->0) {
    if (isTick) {
      _engines[i]->raise();

      if (0 == _timer->getTime()) {
        _engines[i]->reset();
      }
    } else {
      _engines[i]->loop();
    }
  }

  #if LOOP_SLEEP_MS > 0
  delay(LOOP_SLEEP_MS);
  #endif
}


void fail()
{
#ifdef LED_BUILTIN
  uint8_t light = 0;
  for(;;) {
    analogWrite(LED_BUILTIN, light--);
    delay(5);
  }
#endif
}
