#ifndef Bridge_h
#define Bridge_h



#include <ESP8266WiFi.h>
#include "config.h"
#include "macro.h"



class Bridge {

  public:
  struct RelayMessage {
    RelayMessage() : isOk(false) {};
    uint8_t relayId;
    uint8_t pinId;
    bool state;
    bool isLocked;
    bool isNc;
    bool isOk;
  };
  typedef std::function<void(const RelayMessage*, const uint8_t)> TPrintMessageRelayFunction;

  Bridge(Stream &inputStream);
  
  const RelayMessage* getCurrentRelayMessage(void) const { return this->_relayMessage; };
  const uint8_t size(void);
  
  const bool getRelay(const uint8_t relayId, const bool unlock=false) const;
  const bool setRelay(const uint8_t relayId, const bool state, const bool lock=false) const;
  const bool mapRelayToPin(const uint8_t relayId, const uint8_t pinId) const;
  const bool isRelayNc(const uint8_t relayId, const bool isNc) const;

  const bool walkRelayList(TPrintMessageRelayFunction printRelayMessage) const;

  const bool save(void) const;
  const bool reset(void) const;
  const bool sleep(void) const;
  void wakeup(void) const;

  protected:
  const bool _executeNone(String command) const;
  const bool _executeOne(String command) const;
  const bool _executeAll(String command, TPrintMessageRelayFunction printRelayMessage) const;
  const bool _write(String command) const;
  const bool _hasMessageBegin(void) const;
  const bool _seekMessageBegin(void) const;
  const bool _hasMessageEnd(void) const;
  const bool _seekMessageEnd(void) const;
  const bool _readRelayMessage(void) const;
  void _clear(void) const;

  Stream* _stream = nullptr;
  RelayMessage* _relayMessage = nullptr;
  uint8_t _length;
};



#endif
