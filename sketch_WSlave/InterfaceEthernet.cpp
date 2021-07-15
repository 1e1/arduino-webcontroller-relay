#include "InterfaceEthernet.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceEthernet::InterfaceEthernet()
{
  // switch off SD Card
  pinMode(WS_WIRING_SDCARD_SS, OUTPUT);
  digitalWrite(WS_WIRING_SDCARD_SS, WS_WIRING_SDCARD_SS_OFF);

  Ethernet.init(WS_WIRING_ETHERNET_SS);

  #if WS_MAC_TYPE & WS_MAC_TYPE_DYNAMIC
  randomSeed(analogRead(0));

  const uint8_t deviceNumber = random(2, 253);
  #else
  const uint8_t deviceNumber = WS_DEVICE_NUMBER;
  #endif

  byte mac[] = MAC_ADDRESS(deviceNumber);

  Ethernet.begin(mac, WS_DHCP_TIMEOUT_MS);
  LOGLN(Ethernet.localIP());

  #if WS_BONJOUR_MODE == WS_BONJOUR_MODE_STATIC
  EthernetBonjour.begin(WS_DEVICE_NAME);
  
  #elif WS_BONJOUR_MODE == WS_BONJOUR_MODE_DYNAMIC
  char deviceName[] = WS_DEVICE_NAME_MDNS(WS_DEVICE_NAME);

  const uint8_t deviceNameLength = ARRAYLEN(deviceName);
  deviceName[deviceNameLength-1] = InterfaceEthernet::uint8_t2hex(deviceNumber);
  deviceName[deviceNameLength-2] = InterfaceEthernet::uint8_t2hex(deviceNumber >> 4);

  EthernetBonjour.begin(deviceName);
  #endif

  this->_cycleCounter = 0;
  this->_server = new EthernetServer(WS_WEB_PORT);

}




void InterfaceEthernet::begin() const
{
  this->_server->begin();
}


void InterfaceEthernet::loop()
{
  EthernetClient client = this->_server->available();

  this->_listen(&client);
  this->_broadcast();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void InterfaceEthernet::_broadcast()
{
  this->_cycleCounter++;

  if (this->_cycleCounter == 0) {
    Ethernet.maintain();
  }

  #if WS_BONJOUR_MODE != WS_BONJOUR_MODE_NONE
  if ((this->_cycleCounter & 0x1F) == 1) {
    EthernetBonjour.run();
  }
  #endif
}


const char InterfaceEthernet::_uint8_t2hex(const uint8_t i)
{  
  return "0123456789ABCDEF"[0x0F & i];
}
