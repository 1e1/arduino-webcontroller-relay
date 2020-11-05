#ifndef Configuration_h
#define Configuration_h



#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <list>
#include "config.h"
#include "macro.h"



class Configuration {

  public:
  typedef enum { T_false=0, T_true=1, T_indeterminate=-1 } TriState;

  struct Acl {
    char* username;
    char* password;
    bool isSafeMode;
    bool canAutoRestart;
  };

  struct WifiAp {
    char* ssid;
    char* password;
    uint8_t channel: 4;
    bool isHidden;
  };

  struct Global {
    Acl acl;
    WifiAp wifiAp;
  };

  struct WifiStation {
    char* ssid;
    char* password;
  };

  struct Relay {
    uint8_t id;
    TriState onConnect;
    char* name;
  };

  Configuration(FS &fs);

  void begin(void);
  void setSafeMode(const bool isSafeMode=true);
  Global* getGlobal(void) { return &this->_global; };
  const std::list<WifiStation> getWifiStationList(void);
  const std::list<Relay> getRelayList(void);

  protected:
  fs::FS* _fs = nullptr;
  Global _global;

  DynamicJsonDocument _open(const char* filename);
  void _loadGlobal(void);

};



#endif
