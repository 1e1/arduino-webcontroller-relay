#ifndef EngineEthernet_H_
#define EngineEthernet_H_

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Ethernet.h>
#include <EthernetBonjour.h>
#include "config.h"
#include "macro.h"
#include "AbstractStream.h"
#include "Relay.h"
#include "_webApp.h"



#define CRLF              "\r\n"
#define HEADER_START      F("HTTP/1.0 200" CRLF "Content-Type: text/")
#define HEADER_END_ACTION F("plain" CRLF CRLF)
#define HEADER_END_HELP   F("html" CRLF "Content-Encoding: gzip" CRLF CRLF)



class InterfaceEthernet : public AbstractStream {

  public:
  InterfaceEthernet();

  void begin(); // call it in setup()
  void loop();  // call it once in the main loop()
  void raise();
  void reset();

  protected:
  EthernetServer* _webServer;

};


#endif
