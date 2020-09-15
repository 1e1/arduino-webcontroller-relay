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
#include "PowerManager.h"
#include "InterfaceEthernet.h"
#include "InterfaceSerial.h"



/** ===================== **/



static PowerManager* _powerManager;
static AbstractInterface* _engines[WS_INTERFACE_COUNT(WS_INTERFACE)];




void setup()
{
  PowerManager::free();

  BUSYLED_WORK;
  DEBUG_START();
  LOGLN(F("DEBUG ON"));

  Relay::begin();

  uint8_t i = ARRAYLEN(_engines);

  #if WS_INTERFACE & WS_INTERFACE_USB
  --i;
  _engines[i] = new InterfaceSerial();
  _engines[i]->begin();
  #endif

  #if WS_INTERFACE & WS_INTERFACE_ETHERNET
  --i;
  _engines[i] = new InterfaceEthernet();
  _engines[i]->begin();
  #endif

  BUSYLED_HIGH;
  LOGLN(F("! change the Serial speed !"));
  WAIT(5000);
  BUSYLED_WORK;

  const PowerManager::Frequency f = PowerManager::Frequency::F_2MHz;
  _powerManager = new PowerManager(f);
  _powerManager->begin();

  LOGLN(F("CONFIGURED"));
}


void loop()
{
  BUSYLED_NONE;
  uint8_t i = ARRAYLEN(_engines);

  while (i-->0) {
    _engines[i]->loop();
  }

  BUSYLED_IDLE;
  _powerManager->update();
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
