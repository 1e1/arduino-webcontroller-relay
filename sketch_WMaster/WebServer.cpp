#include "WebServer.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



#if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
static BearSSL::ESP8266WebServerSecure _server(WM_WEB_PORT);
static BearSSL::ServerSessions _serverCache(WM_WEB_SERVER_CACHE_SIZE);

#else
static ESP8266WebServer _server(WM_WEB_PORT);
#endif


static const char TEXT_HTML[] PROGMEM = "text/html";
static const char TEXT_JSON[] PROGMEM = "text/json";
static const char PLAIN[] PROGMEM = "plain";

static Bridge* _bridge;
static fs::FS* _fs = nullptr;
static char* _username = new char[1]; // { '\0' };
static char* _password = new char[1]; // { '\0' };

static constexpr const char _jsonLine[] = "{\"i\":##,\"s\":#,\"l\":#,\"p\":##,\"n\":#}";
static const size_t _jsonLineLength = strlen(_jsonLine);

static String _relayMessages;





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


void WebServer::setAuthentication(String username, String password)
{
  _username = new char[username.length() + 1];
  strcpy(_username, username.c_str());

  _password = new char[password.length() + 1];
  strcpy(_password, password.c_str());

  LOG("'"); LOG(_username); LOG(F("' : '")); LOG(_password); LOGLN("'");
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
  #if WM_COMPONENT & WM_COMPONENT_API
  _server.on("/", HTTP_GET, []() {
    WebServer::_streamHtml(WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
  }); 

  _server.on("/api/r", HTTP_GET, []() { 
    WebServer::_streamJson(WM_CONFIG_RELAY_PATH, "[]"); 
  });
  #endif

  _server.on("/portal", HTTP_GET, []() {
    if (WebServer::_isAllowed()) {
      WebServer::_streamHtml(WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
    }
  });
  
  _server.on("/about", HTTP_GET, []() {
    _server.sendHeader(F("X-Heap"), String(ESP.getFreeHeap()));
    _server.send(
      200, 
      FPSTR(TEXT_JSON), 
      F("{\"repo\":\"" SCM_REPO "\",\"hash\":\"" SCM_HASH "\",\"date\":\"" SCM_DATE "\",\"chan\":\"" SCM_CHAN "\"}")
    );
  });
  
  _server.onNotFound([](){
    #if WM_COMPONENT & WM_COMPONENT_API
    switch (_server.uri().charAt(1)) {
      case 'a':
        LOGLN(F("handle API"));
        return WebServer::_handleApi();
      case 'c': 
        LOGLN(F("handle Cfg"));
        return WebServer::_handleCfg();
    }

    LOGLN(F("handle NotFound"));
    
    _server.send(404);

    #else
    return WebServer::_handleCfg();
    #endif
  });

  #if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
  LOG(F("certificate "));
  //_server.getServer().setNoDelay(true);
  _server.getServer().setCache(&_serverCache);

  if (certificate::serverCertType == certificate::CertType::CT_ECC) {
    LOGLN(F("ECC"));
    _server.getServer().setECCert(new BearSSL::X509List(certificate::serverCert), BR_KEYTYPE_KEYX|BR_KEYTYPE_SIGN, new BearSSL::PrivateKey(certificate::serverKey));
  } else if(certificate::serverCertType == certificate::CertType::CT_RSA) {
    LOGLN(F("RSA"));
    _server.getServer().setRSACert(new BearSSL::X509List(certificate::serverCert), new BearSSL::PrivateKey(certificate::serverKey));
  } else {
    LOGLN(F("ERROR"));
  }
  #endif

  _server.begin();
}


const bool WebServer::_isAllowed()
{
  if (_username[0] != '\0' && _password[0] != '\0') {
    if (!_server.authenticate(_username, _password)) {
      _server.requestAuthentication();

      return false;
    }
  }

  return true;
}


void WebServer::_handleCfg()
{
  if (!WebServer::_isAllowed()) {
    return;
  }

  String uri = _server.uri();
  if (uri.startsWith(F("/cfg/"))) {
    uri.remove(0, String("/cfg/").length());

    if (_server.method() == HTTP_DELETE && uri.equals(F("reboot"))) {
      _server.send(200, FPSTR(TEXT_JSON), F("\"reboot\""));
      LOGLN(F("** RESTART **"));
      ESP.restart();
    }

    const char cfg = uri.charAt(0);
    const bool isPost = _server.method() == HTTP_POST;

    switch (cfg) {
      case 'g':
        return _uploadAndStreamJson(WM_CONFIG_GLOBAL_PATH, "{}", isPost);

      case 'r':
        return _uploadAndStreamJson(WM_CONFIG_RELAY_PATH, "[]", isPost);

      case 'w':
        return _uploadAndStreamJson(WM_CONFIG_WIFI_PATH, "[]", isPost);

      case 'p':
        if (isPost) {
          WebServer::_writeSerialJson();
          yield();
        }
        
        return WebServer::_readSerialJson();
    }
  }

  _server.send(404);
}


void WebServer::_handleApi()
{
  String uri = _server.uri();
  if (uri.startsWith(F("/api/r/"))) {
    uri.remove(0, String("/api/r/").length());
    const uint8_t relayId = uri.toInt();
    
    switch (_server.method()) {
      case HTTP_GET:
        return _bridge->getRelay(relayId)
          ? WebServer::_sendRelayMessage(_bridge->getCurrentRelayMessage())
          : _server.send(500);
          ;

      case HTTP_PUT:
        if (!_server.hasArg(FPSTR(PLAIN))) {
          return _server.send(400);
        }
        
        String payload = _server.arg(FPSTR(PLAIN));
        StaticJsonDocument<WM_API_BUFFER_SIZE> jsonBuffer;
        auto error = deserializeJson(jsonBuffer, payload, DeserializationOption::NestingLimit(2));
        LOGLN(payload);
        
        if (!error) {
          const bool state = jsonBuffer["s"].as<bool>();
          
          if (_bridge->setRelay(relayId, state)) {
            WebServer::_sendRelayMessage(_bridge->getCurrentRelayMessage());
          }
        }

        return _server.send(500);
    }
  }

  _server.send(404);
}


void WebServer::_uploadAndStreamJson(const char* path, const char* defaultValue, const bool isUpload)
{
  if (isUpload) {
    WebServer::_uploadJson(path);
    yield();
  }

  WebServer::_streamJson(path, defaultValue);
}


void WebServer::_streamHtml(const char* path)
{
  _server.sendHeader(F("Content-Encoding"), F(WM_WEB_FILE_EXT));
  _server.sendHeader(F("Cache-Control"), F("max-age=86400"));

  File file = _fs->open(path, "r");
  _server.streamFile(file, FPSTR(TEXT_HTML));
  file.close();
}


void WebServer::_streamJson(const char* path, const char* defaultValue)
{
  if (!_fs->exists(path)) {
    return _server.send(200, FPSTR(TEXT_JSON), defaultValue);
  }

  File file = _fs->open(path, "r");
  _server.streamFile(file, FPSTR(TEXT_JSON));
  file.close();
}


void WebServer::_sendRelayMessage(const Bridge::RelayMessage* relay)
{
  if (!relay->isOk) {
    return _server.send(500);
  }

  _server.send(200, FPSTR(TEXT_JSON), relay->state ? F("{\"s\":1}") : F("{\"s\":0}"));
}


void WebServer::_uploadJson(const char* path)
{
  if (!_server.hasArg(FPSTR(PLAIN))) {
    return _server.send(400);
  }

  String payload = _server.arg(FPSTR(PLAIN));
  DynamicJsonDocument jsonBuffer(WM_CONFIG_BUFFER_SIZE);
  auto error = deserializeJson(jsonBuffer, payload, DeserializationOption::NestingLimit(2));
  LOGLN(payload);
  
  if (!error) {
    File file = _fs->open(path, "w");
    serializeJson(jsonBuffer, file);
    file.close();
  }
}


void WebServer::_readSerialJson()
{
  _bridge->wakeup();

  uint8_t i = _bridge->size();

  if (_relayMessages.length() == 0) {
    _relayMessages.reserve(2+ i * (_jsonLineLength+1)); // [ $jsonLine}(,$jsonLine)* ]
    _relayMessages ='[';
  
    while (i-->0) {
      _relayMessages += _jsonLine;
      if (i != 0) {
        _relayMessages += ',';
      }
    }

    _relayMessages += ']';
  
    i = _bridge->size();
  }

  _bridge->walkRelayList([](const Bridge::RelayMessage* relay, const uint8_t index) {
   yield();
   if (relay->isOk) {
      const unsigned int ptr = 1+ index *(_jsonLineLength+1);
      uint8_t dec;

      dec = relay->relayId /10;
      _relayMessages[ptr+5] = dec != 0 ? '0' + dec : ' ';
      _relayMessages[ptr+6] = '0' + (relay->relayId - 10* dec);

      dec = relay->pinId /10;
      _relayMessages[ptr+24] = dec != 0 ? '0' + dec : ' ';
      _relayMessages[ptr+25] = '0' + (relay->pinId - 10* dec);

      _relayMessages[ptr+12] = '0' + relay->state;
      _relayMessages[ptr+18] = '0' + relay->isLocked;
      _relayMessages[ptr+31] = '0' + relay->isNc;
    }
  });

  _server.send(200, FPSTR(TEXT_JSON), _relayMessages);
}


void WebServer::_writeSerialJson()
{
  if (!_server.hasArg(FPSTR(PLAIN))) {
    return _server.send(400);
  }

  _bridge->wakeup();

  JsonArray root;
  {
    DynamicJsonDocument doc(WM_CORE_BUFFER_SIZE);
    String payload = _server.arg(FPSTR(PLAIN));
    auto error = deserializeJson(doc, payload, DeserializationOption::NestingLimit(2));
    doc.shrinkToFit();

    root = doc.as<JsonArray>();
  }

  for (JsonObject o : root) {
    const uint8_t relayId = o["i"].as<uint8_t>();
    const uint8_t pinId   = o["p"].as<uint8_t>();
    const bool    isNc    = o["n"].as<bool>();

    _bridge->mapRelayToPin(relayId, pinId);
    _bridge->isRelayNc(relayId, isNc);
  }

  _bridge->save();
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
