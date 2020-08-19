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
#include <avr/pgmspace.h>
#include <SPI.h>
#include <Ethernet.h>
#include "macro.h"
#include "config.h"
#include "InterfaceEthernet.h"
#include "InterfaceSerial.h"
#include "FastTimer.h"
#include "LedBuiltIn.h"



/** ===================== **/
/**       observers       **/
/** ===================== **/

// Restarts program from beginning but does not reset the peripherals and registers
__attribute__((always_inline)) inline void software_reset();
void software_reset()
{
  asm volatile ("jmp 0");
}




/** ===================== **/



static FastTimer* _timer;
static AbstractInterface* _engines[MODE_SERIAL_COUNT(MODE_SERIAL)];




void setup()
{
  BUSYLED_IDLE;
  DEBUG_START();
  LOGLN(F("DEBUG ON"));

  const FastTimer::Precision p = FastTimer::Precision::P65s_4h;
  _timer = new FastTimer(p);

  Relay::begin();

  uint8_t i = MODE_SERIAL_COUNT(MODE_SERIAL);

  #if MODE_SERIAL & MODE_SERIAL_ETHERNET
  randomSeed(analogRead(0));

  const uint8_t deviceNumber = random(2, 253);
  byte mac[] = MAC_ADDRESS(deviceNumber);

  Ethernet.begin(mac, DHCP_TIMEOUT_MS);
  LOGLN(Ethernet.localIP());

  _engines[--i] = new InterfaceEthernet();
  _engines[i]->begin();
  #endif

  #if MODE_SERIAL & MODE_SERIAL_USB
  _engines[--i] = new InterfaceSerial();
  _engines[i]->begin();
  #endif

  LOGLN(F("CONFIGURED"));
  BUSYLED_NONE;
}


void loop()
{
  const bool isTick = _timer->update();

  uint8_t i = MODE_SERIAL_COUNT(MODE_SERIAL);

  while (i-->0) {
    if (isTick) {
      // broadcast
      _engines[i]->raise();

      if (0 == _timer->getTime()) {
        // maintain
        _engines[i]->reset();
      }
    }

    _engines[i]->loop();
  }
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
