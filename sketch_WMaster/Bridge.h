#ifndef Bridge_h
#define Bridge_h



#include <ESP8266WiFi.h>
#include <list>
#include "config.h"
#include "macro.h"



class Bridge {

  public:
  struct RelayMessage {
    uint8_t relayId;
    uint8_t pinId;
    bool state;
    bool isLocked;
    bool isNc;
    bool isOk;
  };

  Bridge(Stream &inputStream);

  const RelayMessage getRelay(const uint8_t relayId, const bool unlock=false);
  const RelayMessage setRelay(const uint8_t relayId, const bool state, const bool lock=false);
  const RelayMessage mapRelayToPin(const uint8_t relayId, const uint8_t pinId);
  const RelayMessage isRelayNc(const uint8_t relayId, const bool isNc);

  const std::list<RelayMessage> getRelays();

  void save();
  void reset();
  void sleep();
  void wakeup();

  protected:
  void _executeNone(String command);
  const RelayMessage _executeOne(String command);
  const std::list<RelayMessage> _executeAll(String command);
  const bool _write(String command);
  const bool _hasMessageBegin();
  const bool _seekMessageBegin();
  const bool _hasMessageEnd();
  const bool _seekMessageEnd();
  const RelayMessage _read();
  void _clear();

  Stream* _currentStream = nullptr;
};



#endif
