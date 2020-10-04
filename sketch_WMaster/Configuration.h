#ifndef Configuration_h
#define Configuration_h



#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Vector.h>
#include "config.h"
#include "macro.h"



class Configuration {

  public:
  struct Global {
    const char* wwwUsername;
    const char* wwwPassword;
    const char* wifiSsid;
    const char* wifiPassword;
    uint8_t wifiChannel: 4;
    bool wifiIsHidden;
    bool autoRestart;
  };

  struct Wifi {
    const char* ssid;
    const char* password;
  };

  struct Device {
    const char* mac; // char mac[18]
    const char* command;
  };

  void begin();
  const bool exists();
  void erase();
  Global* getGlobal() { return &this->_global; };
  const Vector<Wifi> getWifiList();
  const Vector<Device> getDeviceList();

  protected:
  Global _global;

  DynamicJsonDocument _open(const char* filename);
  void _loadGlobal();

};



#endif
