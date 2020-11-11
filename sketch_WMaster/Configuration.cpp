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
  if (_fs->exists(WM_CONFIG_GLOBAL_PATH)) {
    this->_loadGlobal();
  } else {
    this->setSafeMode();
  }
}


void Configuration::setSafeMode(const bool isSafeMode)
{
  if (isSafeMode) {
    Configuration::Global g {
      .acl = {
        .username = "",
        .password = "",
        .isSafeMode = true,
        .canAutoRestart = false,
      },
      .wifiAp = {
        .ssid = WM_WIFI_SSID,
        .password = {},
        .channel = 1,
        .isHidden = false,
      }
    };

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
        .ssid = o["n"].as<String>(),
        .password = o["p"].as<String>(),
      };

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
        .name = o["n"].as<String>(),
      };
      
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
  DynamicJsonDocument doc = this->_open(WM_CONFIG_GLOBAL_PATH);
  JsonObject root = doc.as<JsonObject>();
  
  Configuration::Global g {
    .acl = {
      .username = root["u"].as<String>(),
      .password = root["w"].as<String>(),
      .isSafeMode = false,
      .canAutoRestart = root["r"].as<bool>(),
    },
    .wifiAp = {
      .ssid = root["n"].as<String>(),
      .password = root["p"].as<String>(),
      .channel = root["c"].as<uint8_t>(),
      .isHidden = root["h"].as<bool>(),
    }
  };

  this->_global = g;
}
