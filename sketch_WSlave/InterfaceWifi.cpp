#include "InterfaceWifi.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceWifi::InterfaceWifi()
{
  char ssid[] = WS_WIFI_SSID;
  char pass[] = WS_WIFI_PASSWORD;

  WiFi.begin(ssid, pass);
  LOGLN(WiFi.localIP());

  this->_server = new WiFiServer(WS_WEB_PORT);
}




void InterfaceWifi::begin()
{
  this->_server->begin();
}


void InterfaceWifi::loop()
{
  WiFiClient client = this->_server->available();

  this->_listen(&client);
}
