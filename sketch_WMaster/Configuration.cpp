#include "Configuration.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




Configuration::Configuration(FS &fs)
{
  this->_fs = &fs;
}


void Configuration::begin()
{
  this->_loadGlobal();
}


void Configuration::setSafeMode(const bool isSafeMode)
{
  if (isSafeMode) {
    Configuration::Global g {
      .acl = {
        .username = NULL,
        .password = NULL,
        .isSafeMode = true,
        .canAutoRestart = false
      },
      .wifiAp = {
        .ssid = new char[strlen(WM_WIFI_SSID)],
        .password = NULL,
        .channel = 1,
        .isHidden = false
      }
    };

    strcpy(g.wifiAp.ssid, WM_WIFI_SSID);

    this->_global = g;
  }
}


const std::list<Configuration::WifiStation> Configuration::getWifiStationList()
{
  std::list<Configuration::WifiStation> wifiStationList;
  if (_fs->exists(WM_CONFIG_WIFI_PATH)) {
    DynamicJsonDocument doc = this->_open(WM_CONFIG_WIFI_PATH);
    JsonArray root = doc.as<JsonArray>();

    for (JsonObject o : root) {
      Configuration::WifiStation wifi {
        .ssid = new char[strlen(o["n"])],
        .password = new char[strlen(o["p"])]
      };
    
      strcpy(wifi.ssid, o["n"]);
      strcpy(wifi.password, o["p"]);

      wifiStationList.emplace_back(wifi);
    }
  }

  return wifiStationList;
}


const std::list<Configuration::Relay> Configuration::getRelayList()
{
  std::list<Configuration::Relay> relayList;
  if (_fs->exists(WM_CONFIG_RELAY_PATH)) {
    DynamicJsonDocument doc = this->_open(WM_CONFIG_RELAY_PATH);
    JsonArray root = doc.as<JsonArray>();

    for (JsonObject o : root) {
      Configuration::Relay relay {
        .id = o["i"].as<uint8_t>(),
        .onConnect = static_cast<TriState>(o["a"].as<int>()),
        .name = new char[strlen(o["n"])]
      };

      strcpy(relay.name, o["n"]);
      
      relayList.emplace_back(relay);
    }
  }

  return relayList;
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




DynamicJsonDocument Configuration::_open(const char* filename)
{
  File file = _fs->open(filename, "r"); // "w+"
  DynamicJsonDocument doc(WM_CONFIG_BUFFER_SIZE);
  deserializeJson(doc, file, DeserializationOption::NestingLimit(2));
  file.close();

  return doc;
}


void Configuration::_loadGlobal()
{
  if (_fs->exists(WM_CONFIG_GLOBAL_PATH)) {
    DynamicJsonDocument doc = this->_open(WM_CONFIG_GLOBAL_PATH);
    JsonObject root = doc.as<JsonObject>();
    
    Configuration::Global g {
      .acl = {
        .username = new char[strlen(root["u"])],
        .password = new char[strlen(root["w"])],
        .isSafeMode = false,
        .canAutoRestart = root["r"].as<bool>()
      },
      .wifiAp = {
        .ssid = new char[strlen(root["n"])],
        .password = new char[strlen(root["p"])],
        .channel = root["c"].as<uint8_t>(),
        .isHidden = root["h"].as<bool>()
      }
    };

    strcpy(g.acl.username, root["u"]);
    strcpy(g.acl.password, root["w"]);
    strcpy(g.wifiAp.ssid, root["n"]);
    strcpy(g.wifiAp.password, root["p"]);

    this->_global = g;
    /* */
  }
}
