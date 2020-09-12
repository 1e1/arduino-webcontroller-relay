#include "InterfaceEthernet.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/




LONGBYTES(webpage) = WEBPAGE;
static const size_t webpage_len = ARRAYLEN(webpage);



static const char uint8_t2hex(const uint8_t i)
{  
  return "0123456789ABCDEF"[0x0F & i];
}




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceEthernet::InterfaceEthernet()
{
  #if WS_TYPE_MAC & WS_TYPE_MAC_DYNAMIC
  randomSeed(analogRead(0));

  const uint8_t deviceNumber = random(2, 253);
  #else
  const uint8_t deviceNumber = DEVICE_NUMBER;
  #endif

  byte mac[] = MAC_ADDRESS(deviceNumber);

  Ethernet.begin(mac, DHCP_TIMEOUT_MS);
  LOGLN(Ethernet.localIP());

  #if WS_MODE_BONJOUR == WS_MODE_BONJOUR_STATIC
  EthernetBonjour.begin(DEVICE_NAME);
  
  #elif WS_MODE_BONJOUR == WS_MODE_BONJOUR_DYNAMIC
  char deviceName[] = DEVICE_NAME_MDNS(DEVICE_NAME);

  const uint8_t deviceNameLength = ARRAYLEN(deviceName);
  deviceName[deviceNameLength-1] = uint8_t2hex(deviceNumber);
  deviceName[deviceNameLength-2] = uint8_t2hex(deviceNumber >> 4);

  EthernetBonjour.begin(deviceName);
  #endif

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
      client.print(F(WS_HEADER_BEGIN_P));

      #if WS_MODE_VERBOSE & WS_MODE_VERBOSE_WEBAPP
      if (this->read()) {
        client.print(F(WS_HEADER_END_ACTION_P));
        this->process();
      } else {
        client.print(F(WS_HEADER_END_HELP_P));
        for (int i = 0; i<webpage_len; i++) {
          client.write(pgm_read_byte_near(webpage + i));
        }
      }
      #else
      if (this->read()) {
        client.print(F(WS_HEADER_END_ACTION_P));
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
  #if WS_MODE_BONJOUR != WS_MODE_BONJOUR_NONE
  EthernetBonjour.run();
  #endif
}


void InterfaceEthernet::reset()
{
  Ethernet.maintain();
}
