#ifndef InterfaceWifi_H_
#define InterfaceWifi_H_

#include <Arduino.h>
#include <WiFi.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterfaceWebApp.h"



class InterfaceWifi : public AbstractInterfaceWebApp {

  public:
  InterfaceWifi(void);

  void begin(void) const;
  void loop(void);

  protected:
  WiFiServer* _server;

};


#endif
