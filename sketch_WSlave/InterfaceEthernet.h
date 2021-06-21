#ifndef InterfaceEthernet_H_
#define InterfaceEthernet_H_

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterfaceWebApp.h"



class InterfaceEthernet : public AbstractInterfaceWebApp {

  public:
  InterfaceEthernet(void);

  void begin(void) const;
  void loop(void);

  protected:
  void _broadcast(void);

  uint8_t _cycleCounter;
  EthernetServer* _server;

  static const char _uint8_t2hex(const uint8_t i);

};


#endif
