#include "Configuration.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void Configuration::begin()
{
  LittleFS.begin();

  this->_loadGlobal();
}


const bool Configuration::exists()
{
 return LittleFS.exists(WM_CONFIG_GLOBAL_PATH);
}


void Configuration::erase()
{
  LittleFS.remove(WM_CONFIG_GLOBAL_PATH);
}


const Vector<Configuration::Wifi> Configuration::getWifiList()
{
  DynamicJsonDocument doc = this->_open(WM_CONFIG_WIFI_PATH);
  JsonArray root = doc.as<JsonArray>();
  Vector<Configuration::Wifi> vector;

  for (JsonObject o : root) {
    Configuration::Wifi wifi;

    wifi.ssid = o["n"].as<const char*>();
    wifi.password = o["p"].as<const char*>();

    vector.push_back(wifi);
  }

  return vector;
}


const Vector<Configuration::Device> Configuration::getDeviceList()
{
  DynamicJsonDocument doc = this->_open(WM_CONFIG_DEVICE_PATH);
  JsonArray root = doc.as<JsonArray>();
  Vector<Configuration::Device> vector;

  for (JsonObject o  : root) {
    Configuration::Device device;

    device.mac = o["m"].as<const char*>();
    device.command = o["c"].as<const char*>();

    vector.push_back(device);
  }
  
  return vector;
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




DynamicJsonDocument Configuration::_open(const char* filename)
{
  File file = LittleFS.open(filename, "r");
  DynamicJsonDocument doc(WM_CONFIG_BUFFER_SIZE);
  deserializeJson(doc, file, DeserializationOption::NestingLimit(1));
  file.close();

  return doc;
}


void Configuration::_loadGlobal()
{
  if (LittleFS.exists(WM_CONFIG_GLOBAL_PATH)) {
    DynamicJsonDocument doc = this->_open(WM_CONFIG_GLOBAL_PATH);
    JsonObject root = doc.as<JsonObject>();

    this->_global.wwwUsername = root["u"].as<const char*>();
    this->_global.wwwPassword = root["w"].as<const char*>();
    this->_global.wifiSsid = root["n"].as<const char*>();
    this->_global.wifiPassword = root["p"].as<const char*>();
    this->_global.wifiChannel = root["c"].as<uint8_t>();
    this->_global.wifiIsHidden = root["h"].as<bool>();
    this->_global.autoRestart = root["r"].as<bool>();
  } else {
    this->_global.wwwUsername = NULL;
    this->_global.wwwPassword = NULL;
    this->_global.wifiSsid = WM_WIFI_SSID;
    this->_global.wifiPassword = NULL;
    this->_global.wifiChannel = 1;
    this->_global.wifiIsHidden = false;
    this->_global.autoRestart = false;
  }
}
