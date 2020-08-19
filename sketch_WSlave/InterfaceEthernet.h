#ifndef EngineEthernet_H_
#define EngineEthernet_H_

#include <Arduino.h>
#include <avr/pgmspace.h>
#include <Ethernet.h>
#include "config.h"
#include "macro.h"
#include "AbstractStream.h"
#include "Relay.h"
#include "_webApp.h"



#define ETH_SPI_D0_UNO    10
#define ETH_SPI_D1_UNO    11
#define ETH_SPI_D2_UNO    12
#define ETH_SPI_D3_UNO    13
#define ETH_SPI_D1_MEGA   50
#define ETH_SPI_D2_MEGA   51
#define ETH_SPI_D3_MEGA   52
#define ETH_SPI_SS_MEGA   53
#define ETH_SELECT_W5100  10
#define ETH_SELECT_SD     4

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
