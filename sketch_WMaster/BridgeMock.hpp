#ifndef BridgeMock_h
#define BridgeMock_H_



#include <ESP8266WiFi.h>
#include "config.h"
#include "macro.h"
#include "Bridge.h"



class BridgeMock : public Bridge {

  public:
  BridgeMock(void) {};

  const RelayMessage* getCurrentRelayMessage(void) 
  { 
    this->_generateRelayMessage();
    return this->_relayMessage; 
  };
  const uint8_t size(void) const { return 16; };
  
  const bool getRelay(const uint8_t relayId, const bool unlock=false) const { return true; };
  const bool setRelay(const uint8_t relayId, const bool state, const bool lock=false) const { return true; };
  const bool mapRelayToPin(const uint8_t relayId, const uint8_t pinId) const { return true; };
  const bool isRelayNc(const uint8_t relayId, const bool isNc) const { return true; };

  const bool walkRelayList(TPrintMessageRelayFunction printRelayMessage)
  {
    for(uint8_t i=0; i<16; ++i) {
      this->_generateRelayMessage();
      printRelayMessage(this->_relayMessage, i);
    }

    return true;
  };

  const bool save(void) const;
  const bool reset(void) const;
  const bool sleep(void) const;
  void wakeup(void) const;

  protected:
  void _generateRelayMessage(void)
  {
    delay(1);
    LOGLN(F("_generateRelayMessage"));

    this->_relayMessage->state    = random(0, 2);
    this->_relayMessage->relayId  = random(0, 32);
    this->_relayMessage->isLocked = random(0, 2);
    this->_relayMessage->isNc     = random(0, 2);
    this->_relayMessage->pinId    = random(0, 32);

    this->_relayMessage->isOk     = true;
  }
};



#endif
