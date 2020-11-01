#include "WebServer.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



static BearSSL::ESP8266WebServerSecure _server(WM_WEB_PORT);

static Bridge* _bridge;
static fs::FS* _fs = nullptr;
static const char* _username = NULL;
static const char* _password = NULL;
//static char* _serverKey;
//static char* _serverCert;





/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void WebServer::begin()
{
  this->_setup();
}


void WebServer::loop()
{
  _server.handleClient();
}


void WebServer::setAuthentication(const char* username, const char* password)
{
  _username = username;
  _password = password;
  LOG("'"); LOG(_username); LOG("' : '"); LOG(_password); LOGLN("'");
}


void WebServer::setBridge(Bridge* bridge)
{
  _bridge = bridge;
}


void WebServer::setFs(FS &fs)
{
  _fs = &fs;
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void WebServer::_setup()
{
  _server.on("/", HTTP_GET, []() {
    WebServer::_streamBrotli(WM_WEB_INDEX_PATH, true);
  }); 

  _server.on("/api/r", HTTP_GET, []() { 
    WebServer::_streamJson(WM_CONFIG_RELAY_PATH, "[]", true); 
  });

  _server.on("/portal", HTTP_GET, []() {
    WebServer::_streamBrotli(WM_WEB_PORTAL_PATH, false);
  });

  _server.on("/cfg/g", HTTP_GET, []() {
    WebServer::_streamJson(WM_CONFIG_GLOBAL_PATH, "{}", false);
  });

  _server.on("/cfg/g", HTTP_POST, []() {
    WebServer::_uploadJson(WM_CONFIG_GLOBAL_PATH);
  });

  _server.on("/cfg/w", HTTP_GET, []() { 
    WebServer::_streamJson(WM_CONFIG_WIFI_PATH, "[]", false);
  });

  _server.on("/cfg/w", HTTP_POST, []() {
    WebServer::_uploadJson(WM_CONFIG_WIFI_PATH);
  });

  _server.on("/cfg/r", HTTP_GET, []() { 
    WebServer::_streamJson(WM_CONFIG_RELAY_PATH, "[]", false);
  });

  _server.on("/cfg/r", HTTP_POST, []() {
    WebServer::_uploadJson(WM_CONFIG_RELAY_PATH);
  });
  
  _server.on("/cfg/p", HTTP_GET, []() { 
    WebServer::_readSerialJson(); 
  });

  _server.on("/cfg/p", HTTP_POST, []() {
    WebServer::_writeSerialJson();
  });
  
  _server.on("/cfg/reboot", HTTP_DELETE, []() {
    if (WebServer::_isAllowed()) {
      _server.send(200, "text/json", "\"reboot\"");
      LOGLN(F("** RESTART **"));
      ESP.restart();
    }
  });
  /*
  _server.onNotFound([](){
    LOGLN("handle NotFound");
    _server.send(404, "text/plain", "It is not the page your are looking for");
  });
  */
  //WebServer::_getFileContents(WM_CONFIG_KEY_PATH, _serverKey);
  //WebServer::_getFileContents(WM_CONFIG_CERT_PATH, _serverCert);

  _server.getServer().setRSACert(new BearSSL::X509List(certificate::serverCert), new BearSSL::PrivateKey(certificate::serverKey));
  _server.begin();
}


void WebServer::_handleAll()
{
  const char *uri = _server.uri().c_str();
  const char *prefixUrl = PSTR("/api/r/");

  if (strcmp_P(uri, prefixUrl)) {
    uri += strlen_P(prefixUrl); // skip the prefixUrl and get to the relayId
    uint8_t relayId = atoi(uri);
    
    switch (_server.method()) {
      case HTTP_GET:
        return WebServer::_sendRelayMessage(_bridge->getRelay(relayId));

      case HTTP_PUT:
        if (_server.hasArg("plain")) {
          String payload = _server.arg("plain");
          DynamicJsonDocument doc(WM_CONFIG_BUFFER_SIZE);
          auto error = deserializeJson(doc, payload, DeserializationOption::NestingLimit(2));
          LOGLN(payload);
          
          if (!error) {
            const bool state = doc["s"].as<bool>();
            
            return WebServer::_sendRelayMessage(_bridge->setRelay(relayId, state));
          }
        }
    }
  }

  return _server.send(404);
}


const bool WebServer::_isAllowed()
{
  if (_username != NULL && _password != NULL) {
    if (!_server.authenticate(_username, _password)) {
      _server.requestAuthentication();

      return false;
    }
  }

  return true;
}


void WebServer::_streamBrotli(const char* path, const bool isPublic)
{
  if (isPublic || WebServer::_isAllowed()) {
    _server.sendHeader(String(PSTR("Content-Encoding")), String(PSTR("br")));
    _server.sendHeader(String(PSTR("Cache-Control")), String(PSTR("max-age=86400")));

    File file = _fs->open(path, "r");
    _server.streamFile(file, "text/html");
    file.close();
  }
}


void WebServer::_streamJson(const char* path, const char* defaultValue, const bool isPublic)
{
  if (isPublic || WebServer::_isAllowed()) {
    if (_fs->exists(path)) {
      File file = _fs->open(path, "r");
      _server.streamFile(file, "text/json");
      file.close();

      return;
    }
  }

  _server.send(200, "text/json", defaultValue);
}


void WebServer::_sendRelayMessage(const Bridge::RelayMessage relay)
{
  _server.send(200, "text/json", String("{\"s\":") + String(relay.state) + String("}"));
}


void WebServer::_uploadJson(const char* path)
{
  if (WebServer::_isAllowed()) {
    if (_server.hasArg("plain")) {
      String payload = _server.arg("plain");
      DynamicJsonDocument doc(WM_CONFIG_BUFFER_SIZE);
      auto error = deserializeJson(doc, payload, DeserializationOption::NestingLimit(2));
      LOGLN(payload);
      
      if (!error) {
        File file = _fs->open(path, "w");
        serializeJson(doc, file);
        file.close();
      }
    }

    WebServer::_streamJson(path, "null");
  }
}


void WebServer::_readSerialJson()
{
  if (WebServer::_isAllowed()) {
      bool hasComa = false;
      std::list<Bridge::RelayMessage> relayList = _bridge->getRelays();

      _server.setContentLength(CONTENT_LENGTH_UNKNOWN);
      _server.send(200, "text/json", "");
      _server.sendContent("[");

      for (Bridge::RelayMessage relay : relayList) {
        if (relay.isOk) {
          String relayJson;

          if (hasComa) {
            relayJson = ',';
          }

          relayJson += "{\"i\":";
          relayJson += String(relay.relayId);
          relayJson += ",\"s\":";
          relayJson += String(relay.state);
          relayJson += ",\"l\":";
          relayJson += String(relay.isLocked);
          relayJson += ",\"p\":";
          relayJson += String(relay.pinId);
          relayJson += ",\"n\":";
          relayJson += String(relay.isNc);
          relayJson += "}";

          _server.sendContent(relayJson);

          hasComa = true;
        }
      }

      _server.sendContent("]");
  }
}


void WebServer::_writeSerialJson()
{
  if (WebServer::_isAllowed()) {
    if (_server.hasArg("plain")) {
      _bridge->wakeup();

      StaticJsonDocument<64> filter;
      filter[0]["i"] = true;
      filter[0]["p"] = true;
      filter[0]["n"] = true;

      String payload = _server.arg("plain");
      DynamicJsonDocument doc(JSON_ARRAY_SIZE(WM_RELAY_NB_MAX+1)*4);
      auto error = deserializeJson(doc, payload, DeserializationOption::Filter(filter));
      
      JsonArray root = doc.as<JsonArray>();

      for (JsonObject o : root) {
        const uint8_t relayId = o["i"].as<uint8_t>();
        const uint8_t pinId = o["p"].as<uint8_t>();
        const bool isNc = o["n"].as<bool>();

        _bridge->mapRelayToPin(relayId, pinId);
        _bridge->isRelayNc(relayId, isNc);
      }

      _bridge->save();
    }

    WebServer::_readSerialJson();
  }
}


const size_t WebServer::_getFileContents(const char* path, char* &buffer)
{
  File file = _fs->open(path, "r");
  const size_t size = file.size();
  std::unique_ptr<char[]> buf(new char[size]);
  file.readBytes(buf.get(), size);
  file.close();

  return size;
}
