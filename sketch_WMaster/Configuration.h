#ifndef Configuration_H_
#define Configuration_H_



#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <list>
#include "config.h"
#include "macro.h"



class Configuration {

  public:
  typedef enum { T_indeterminate=-1, T_false=0, T_true=1 } TriState;

  struct Acl {
    String username;
    String password;
    bool isSafeMode;
    bool canAutoRestart;
  };

  struct WifiAp {
    String ssid;
    String password;
    uint8_t channel: 4;
    bool isHidden;
  };

  struct Global {
    Acl acl;
    WifiAp wifiAp;
    //bool isSecure;
  };

  struct WifiStation {
    String ssid;
    String password;
  };

  struct Relay {
    uint8_t id;
    TriState onConnect;
    String name;
  };

  Configuration(FS &fs);

  void begin(void);
  void setSafeMode(const bool isSafeMode=true);
  const Global* getGlobal(void) const { return &this->_global; };
  const std::list<WifiStation> getWifiStationList(void) const;
  const std::list<Relay> getRelayList(void) const;

  protected:
  fs::FS* _fs = nullptr;
  Global _global;

  JsonDocument* _open(const char* filename) const;
  void _loadGlobal(void);

};



#endif
