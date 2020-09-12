#ifndef EngineEthernet_H_
#define EngineEthernet_H_

#include <Arduino.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include "config.h"
#include "macro.h"
#include "AbstractStream.h"
#include "_webApp.h"



class InterfaceEthernet : public AbstractStream {

  public:
  InterfaceEthernet();

  void begin();
  void loop();
  void raise();
  void reset();

  protected:
  EthernetServer* _webServer;

};


#endif
