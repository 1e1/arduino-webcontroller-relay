#ifndef Bridge_H_
#define Bridge_H_



#include <Arduino.h>
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
  uint8_t size(void);
  
  bool getRelay(const uint8_t relayId, const bool unlock=false) const;
  bool setRelay(const uint8_t relayId, const bool state, const bool lock=false) const;
  bool mapRelayToPin(const uint8_t relayId, const uint8_t pinId) const;
  bool isRelayNc(const uint8_t relayId, const bool isNc) const;

  bool walkRelayList(TPrintMessageRelayFunction printRelayMessage) const;
  bool prepareRelayList(void) const;
  bool nextRelay(void) const;

  bool save(void) const;
  bool reset(void) const;
  bool sleep(void) const;
  void wakeup(void) const;

  protected:
  bool _executeNone(String command) const;
  bool _executeOne(String command) const;
  bool _executeAll(String command, TPrintMessageRelayFunction printRelayMessage) const;
  bool _write(String command) const;
  bool _hasMessageBegin(void) const;
  bool _seekMessageBegin(void) const;
  bool _hasMessageEnd(void) const;
  bool _seekMessageEnd(void) const;
  bool _readRelayMessage(void) const;
  bool _timedAvailable(const int bufferSize=1) const;
  void _clear(void) const;

  Stream* _stream = nullptr;
  RelayMessage* _relayMessage = nullptr;
  uint8_t _length;
};



#endif
