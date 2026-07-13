#include "Configuration.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




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
        .username = emptyString,
        .password = emptyString,
        .isSafeMode = true,
        .canAutoRestart = false,
      },
      .wifiAp = {
        .ssid = WM_WIFI_SSID,
        .password = {},
        .channel = 1,
        .isHidden = false,
      },
      //.isSecure = false,
    };

    this->_global = g;
  }
}


const std::list<Configuration::WifiStation> Configuration::getWifiStationList() const
{
  std::list<Configuration::WifiStation> wifiStationList;
  if (_fs->exists(WM_CONFIG_WIFI_PATH)) {
    JsonDocument doc;
    this->_open(WM_CONFIG_WIFI_PATH, doc);
    
    const JsonArray root = doc.as<JsonArray>();

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


const Configuration::GCalendar Configuration::getGCalendar() const
{
  GCalendar gCal;
  if (_fs->exists(WM_CONFIG_GCALENDAR_PATH)) {
    JsonDocument doc;
    this->_open(WM_CONFIG_GCALENDAR_PATH, doc);

    const String name = doc["n"].as<String>();

    gCal = {
      .refreshToken = name.isEmpty() ? "" : doc["t"].as<String>(),
      .calendarName = name,
    };
  }

  return gCal;
}


void Configuration::setGCalendar(const Configuration::GCalendar& gCal) const
{
  JsonDocument doc;
  doc["t"] = gCal.refreshToken;
  doc["n"] = gCal.calendarName;

  File file = _fs->open(WM_CONFIG_GCALENDAR_PATH, "w");
  serializeJson(doc, file);
  file.close();
}


const std::list<Configuration::Relay> Configuration::getRelayList() const
{
  std::list<Configuration::Relay> relayList;
  if (_fs->exists(WM_CONFIG_RELAY_PATH)) {
    JsonDocument doc;
    this->_open(WM_CONFIG_RELAY_PATH, doc);

    const JsonArray root = doc.as<JsonArray>();

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




void Configuration::_open(const char* filename, JsonDocument& doc) const
{
  File file = _fs->open(filename, "r"); // "w+"
  deserializeJson(doc, file, DeserializationOption::NestingLimit(2));
  file.close();
  doc.shrinkToFit();
}


void Configuration::_loadGlobal()
{
  JsonDocument doc;
  this->_open(WM_CONFIG_GLOBAL_PATH, doc);

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
    },
    //.isSecure = root["s"].as<bool>(),
  };

  this->_global = g;
}
