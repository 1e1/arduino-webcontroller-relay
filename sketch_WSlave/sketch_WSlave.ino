/*
 * Wright Slave Relay
 * 
 * https://github.com/1e1/arduino-webcontroller-relay
 *
 */



#include <Arduino.h>
#include "config.h"
#include "macro.h"
#include "InterfaceEthernet.h"
#include "InterfaceSerial.h"
#include "InterfaceUsb.h"
#include "PowerManager.h"



/** ===================== **/



static AbstractInterface* _engines[WS_INTERFACE_COUNT(WS_INTERFACE)];




void setup()
{
  Energy.free();

  BUSYLED_WORK;
  LOG_START();
  LOGLN(F("DEBUG ON"));

  delay(2000);
  Energy.begin();
  Relayboard.begin();

  uint8_t i = ARRAYLEN(_engines);

  #if WS_INTERFACE & WS_INTERFACE_USB
  LOGLN(F("start USB"));
  --i;
  _engines[i] = new InterfaceUsb();
  _engines[i]->begin();
  #endif

  #if WS_INTERFACE & WS_INTERFACE_SERIAL
  LOGLN(F("start Serial"));
  --i;
  _engines[i] = new InterfaceSerial();
  _engines[i]->begin();
  #endif

  #if WS_INTERFACE & WS_INTERFACE_ETHERNET
  LOGLN(F("start Ethernet"));
  --i;
  _engines[i] = new InterfaceEthernet();
  _engines[i]->begin();
  #endif

  #if WS_INTERFACE & WS_INTERFACE_WIFI
  LOGLN(F("start Wifi"));
  --i;
  _engines[i] = new InterfaceWifi();
  _engines[i]->begin();
  #endif

  #if WS_LOG_LEVEL != WS_LOG_LEVEL_OFF
  BUSYLED_HIGH;
  LOGLN(F("! change the Serial speed !"));
  WAIT(1000);
  BUSYLED_WORK;
  #endif

  Energy.lowCpu();

  WAIT(1000);
  LOGLN(F("CONFIGURED"));
}


void loop()
{
  BUSYLED_IDLE;
  uint8_t i = ARRAYLEN(_engines);

  while (i-->0) {
    _engines[i]->loop();
  }

  BUSYLED_NONE;
  Energy.loop();
}
