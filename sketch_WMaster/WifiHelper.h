#ifndef WifiHelper_H_
#define WifiHelper_H_

#include <ESP8266WiFi.h>



class WifiHelper {

  public:
  void begin();
  void loop();

  protected:
  WiFiServer* _server;

};


#endif
