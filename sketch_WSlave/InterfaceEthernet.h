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
  InterfaceEthernet();

  void begin();
  void loop();

  protected:
  void _broadcast();

  EthernetServer* _server;

};


#endif
