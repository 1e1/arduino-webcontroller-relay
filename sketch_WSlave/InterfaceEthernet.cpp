#include "InterfaceEthernet.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




LONGBYTES(webpage)        = WEBPAGE;
static size_t webpage_len = ARRAYLEN(webpage);




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceEthernet::InterfaceEthernet()
{
  this->_webServer = new EthernetServer(WEB_PORT);
}




void InterfaceEthernet::begin()
{
  this->_webServer->begin();
}


void InterfaceEthernet::loop()
{
  EthernetClient client = this->_webServer->available();

  this->setStream(&client);

  if (client.connected()) {
    if (client.available()) {

      #if MODE_VERBOSE & MODE_VERBOSE_WEBAPP
      client.print(HEADER_START);

      if (this->read()) {
        client.print(HEADER_END_ACTION);
        this->process();
      } else {
        client.print(HEADER_END_HELP);
        for (int i = 0; i<webpage_len; i++) {
          client.write(pgm_read_byte_near(webpage + i));
        }
      }
      #else
      if (this->read()) {
        client.print(HEADER_START HEADER_END_ACTION);
        this->process();
      }
      #endif

      this->terminate();
    }

    client.stop();
  }
}


void InterfaceEthernet::raise()
{
}


void InterfaceEthernet::reset()
{
  Ethernet.maintain();
}