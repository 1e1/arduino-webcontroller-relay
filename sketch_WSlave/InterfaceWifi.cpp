#include "InterfaceWifi.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




InterfaceWifi::InterfaceWifi()
{
  #ifndef WS_WIFI_SSID
  WiFi.begin();
  #else
  char ssid[] = WS_WIFI_SSID;
  #ifndef WS_WIFI_PASSWORD
  WiFi.begin(ssid);
  #else
  char pass[] = WS_WIFI_PASSWORD;
  WiFi.begin(ssid, pass);
  #endif
  #endif
  
  LOGLN(WiFi.localIP());

  this->_server = new WiFiServer(WS_WEB_PORT);
}




void InterfaceWifi::begin() const
{
  this->_server->begin();
}


void InterfaceWifi::loop()
{
  WiFiClient client = this->_server->available();

  this->_listen(&client);
}
