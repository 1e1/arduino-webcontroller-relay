#include "WebServerEsp8266.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



#if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
static BearSSL::ServerSessions _serverCache(WM_WEB_SERVER_CACHE_SIZE);
#endif






/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




WebServerEsp8266::WebServerEsp8266(FS &fs, Bridge* bridge)
{
  this->setFs(fs);
  this->setBridge(bridge);

  #if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
  this->_server = new BearSSL::ESP8266WebServerSecure(WM_WEB_PORT_DEFAULT_SECURE);
  
  LOG(F("certificate "));
  this->_server->getServer().setCache(&_serverCache);

  if (certificate::serverCertType == certificate::CertType::CT_ECC) {
    LOGLN(F("ECC"));
    this->_server->getServer().setECCert(new BearSSL::X509List(certificate::serverCert), BR_KEYTYPE_KEYX|BR_KEYTYPE_SIGN, new BearSSL::PrivateKey(certificate::serverKey));
  } else if(certificate::serverCertType == certificate::CertType::CT_RSA) {
    LOGLN(F("RSA"));
    this->_server->getServer().setRSACert(new BearSSL::X509List(certificate::serverCert), new BearSSL::PrivateKey(certificate::serverKey));
  } else {
    LOGLN(F("ERROR"));
  }
  #else
  this->_server = new ESP8266WebServer(WM_WEB_PORT_DEFAULT);
  #endif

  this->_username = new char[1]; // { '\0' };
  this->_password = new char[1]; // { '\0' };
} 


void WebServerEsp8266::begin(void)
{
  this->_setRoutes();
  //this->_server->getServer().setNoDelay(true);
  this->_server->begin();
}


void WebServerEsp8266::loop(void)
{
  this->_server->handleClient();
}


void WebServerEsp8266::setAuthentication(String username, String password)
{
  this->_username = new char[username.length() + 1];
  strcpy(this->_username, username.c_str());

  this->_password = new char[password.length() + 1];
  strcpy(this->_password, password.c_str());

  LOG("'"); LOG(_username); LOG(F("' : '")); LOG(_password); LOGLN("'");
}

/*
void WebServerEsp8266::setMode(const Mode mode)
{
  switch (mode) {
    case MODE_PORTAL:
      return this->_setRoutesPortalOnly();
    
    case MODE_API:
      return this->_setRoutesControllerOnly();
    
    default:
      return this->_setRoutes();
  }
}
*/


/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void WebServerEsp8266::_setRoutes(void)
{
  #if WM_COMPONENT & WM_COMPONENT_API
  this->_server->on("/", HTTP_GET, [=]() {
    this->_redirect(WebServer::ROUTE_HOME);
  });

  this->_server->on(WebServer::ROUTE_HOME, HTTP_GET, [=]() {
    this->_streamHtml(WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
  });

  this->_server->on("/api/r", HTTP_GET, [=]() {
    this->_streamJson(WM_CONFIG_RELAY_PATH, true); 
  });
  #endif

  this->_server->on(WebServer::ROUTE_PORTAL, HTTP_GET, [=]() {
    if (this->_isAllowed()) {
      this->_streamHtml(WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
    }
  });
  
  this->_server->on(WebServer::ROUTE_ABOUT, HTTP_GET, [=]() {
    this->_streamAbout();
  });
  
  this->_server->onNotFound([=]() {
    #if WM_COMPONENT & WM_COMPONENT_API
    switch (this->_server->uri().charAt(1)) {
      case 'a':
        LOGLN(F("handle API"));
        return this->_handleApi();
      case 'c':
        LOGLN(F("handle Cfg"));
        return this->_handleCfg();
    }

    LOGLN(F("handle NotFound"));
    
    this->_server->send(404);

    #else
    this->_handleCfg();
    #endif
  });
}


void WebServerEsp8266::_setRoutesPortalOnly(void)
{
  this->_server->on("/", HTTP_GET, [=]() {
    this->_redirect(WebServer::ROUTE_PORTAL);
  });

  this->_server->on(WebServer::ROUTE_PORTAL, HTTP_GET, [=]() {
    if (this->_isAllowed()) {
      this->_streamHtml(WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
    }
  });
  
  this->_server->on(WebServer::ROUTE_ABOUT, HTTP_GET, [=]() {
    LOGLN(F("/about"));
    this->_streamAbout();
  });
  
  this->_server->onNotFound([=]() {
    LOGLN(F("/notFound"));
    this->_handleCfg();
  });
}


void WebServerEsp8266::_setRoutesControllerOnly(void) const
{
  this->_server->on("/", HTTP_GET, [=]() {
    this->_redirect(WebServer::ROUTE_HOME);
  });

  this->_server->on(WebServer::ROUTE_HOME, HTTP_GET, [=]() {
    this->_streamHtml(WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
  });

  this->_server->on("/api/r", HTTP_GET, [=]() {
    this->_streamJson(WM_CONFIG_RELAY_PATH, true); 
  });
  
  this->_server->on(WebServer::ROUTE_ABOUT, HTTP_GET, [=]() {
    this->_streamAbout();
  });
  
  this->_server->onNotFound([=]() {
    this->_handleApi();
  });
}


const bool WebServerEsp8266::_isAllowed(void) const
{
  if (this->_username[0] != '\0' && this->_password[0] != '\0') {
    if (!this->_server->authenticate(this->_username, this->_password)) {
      this->_server->requestAuthentication();

      return false;
    }
  }

  return true;
}


void WebServerEsp8266::_handleCfg(void)
{
  if (!this->_isAllowed()) {
    return;
  }

  String uri = this->_server->uri();
  if (uri.startsWith(F("/cfg/"))) {
    uri.remove(0, String("/cfg/").length());

    if (this->_server->method() == HTTP_DELETE && uri.equals(F("reboot"))) {
      this->_server->send(200, FPSTR(WebServer::TEXT_JSON), F("\"reboot\""));
      LOGLN(F("** RESTART **"));
      ESP.restart();
    }

    const char cfg = uri.charAt(0);
    const bool isPost = this->_server->method() == HTTP_POST;

    switch (cfg) {
      case 'g':
        return this->_uploadAndStreamJson(WM_CONFIG_GLOBAL_PATH, FPSTR(WebServer::JSON_OBJECT_EMPTY), isPost);

      case 'r':
        return this->_uploadAndStreamJson(WM_CONFIG_RELAY_PATH, FPSTR(WebServer::JSON_ARRAY_EMPTY), isPost);

      case 'w':
        return this->_uploadAndStreamJson(WM_CONFIG_WIFI_PATH, FPSTR(WebServer::JSON_ARRAY_EMPTY), isPost);
        
      case 'p':
        if (isPost) {
          this->_writeSerialJson();
          yield();
        }
        
        return this->_readSerialJson();
    }
  }

  this->_server->sendHeader(FPSTR(WebServer::CACHE_CONTROL), FPSTR(WebServer::MAX_AGE_86400));
  this->_server->send(404);
}


void WebServerEsp8266::_handleApi(void) const
{
  String uri = this->_server->uri();
  if (uri.startsWith(F("/api/r/"))) {
    uri.remove(0, String("/api/r/").length());
    const uint8_t relayId = uri.toInt();
    
    switch (this->_server->method()) {
      case HTTP_GET:
        return _bridge->getRelay(relayId)
          ? this->_sendRelayMessage(_bridge->getCurrentRelayMessage())
          : this->_server->send(500);
          ;

      case HTTP_PUT:
        if (!this->_server->hasArg(FPSTR(WebServer::PLAIN))) {
          return this->_server->send(400);
        }
        
        String payload = this->_server->arg(FPSTR(WebServer::PLAIN));
        StaticJsonDocument<WM_API_BUFFER_SIZE> jsonBuffer;
        DeserializationError error = deserializeJson(jsonBuffer, payload, DeserializationOption::NestingLimit(2));
        LOGLN(payload);
        
        if (!error) {
          const bool state = jsonBuffer["s"].as<bool>();
          
          if (_bridge->setRelay(relayId, state)) {
            return this->_sendRelayMessage(_bridge->getCurrentRelayMessage());
          }
        }

        return this->_server->send(500);
    }
  }

  this->_server->sendHeader(FPSTR(WebServer::CACHE_CONTROL), FPSTR(WebServer::MAX_AGE_86400));
  this->_server->send(404);
}


void WebServerEsp8266::_uploadAndStreamJson(const char* path, const bool isArray, const bool isUpload) const
{
  if (isUpload) {
    this->_uploadJson(path);
    yield();
  }

  this->_streamJson(path, isArray);
}


void WebServerEsp8266::_redirect(String uri) const
{
    this->_server->sendHeader(FPSTR(WebServer::LOCATION), uri, true);
    this->_server->send(307, FPSTR(WebServer::TEXT_HTML), emptyString);
    this->_server->client().stop();
}


void WebServerEsp8266::_streamAbout(void) const
{
  this->_server->sendHeader(FPSTR(WebServer::X_HEAP), String(ESP.getFreeHeap()));
  this->_server->sendHeader(FPSTR(WebServer::X_UPTIME), String(millis()));
  this->_server->send(
    200, 
    FPSTR(WebServer::TEXT_JSON), 
    F("{\"repo\":\"" SCM_REPO "\",\"hash\":\"" SCM_HASH "\",\"date\":\"" SCM_DATE "\",\"chan\":\"" SCM_CHAN "\"}")
  );
}


void WebServerEsp8266::_streamHtml(const char* path) const
{
  if (!this->_fs->exists(path)) {
    return this->_server->send(503);
  }

  this->_server->sendHeader(FPSTR(WebServer::CONTENT_ENCODING), F(WM_WEB_FILE_EXT));
  this->_server->sendHeader(FPSTR(WebServer::CACHE_CONTROL), FPSTR(WebServer::MAX_AGE_86400));

  File file = this->_fs->open(path, "r");
  this->_server->streamFile(file, FPSTR(WebServer::TEXT_HTML));
  file.close();
}


void WebServerEsp8266::_streamJson(const char* path, const bool isArray) const
{
  if (!this->_fs->exists(path)) {
    return this->_server->send(200, FPSTR(WebServer::TEXT_JSON), isArray ? F("[]") : F("{}"));
  }

  File file = this->_fs->open(path, "r");
  this->_server->streamFile(file, FPSTR(WebServer::TEXT_JSON));
  file.close();
}


void WebServerEsp8266::_sendRelayMessage(const Bridge::RelayMessage* relay) const
{
  if (!relay->isOk) {
    return this->_server->send(500);
  }

  this->_server->send(200, FPSTR(WebServer::TEXT_JSON), relay->state ? F("{\"s\":1}") : F("{\"s\":0}"));
}


void WebServerEsp8266::_uploadJson(const char* path) const
{
  if (!this->_server->hasArg(FPSTR(WebServer::PLAIN))) {
    return this->_server->send(400);
  }

  String payload = this->_server->arg(FPSTR(WebServer::PLAIN));
  DynamicJsonDocument jsonBuffer(WM_CONFIG_BUFFER_SIZE);
  DeserializationError error = deserializeJson(jsonBuffer, payload, DeserializationOption::NestingLimit(2));
  LOGLN(payload);
  
  if (!error) {
    File file = this->_fs->open(path, "w");
    serializeJson(jsonBuffer, file);
    file.close();
  }
}


void WebServerEsp8266::_readSerialJson(void)
{
  const unsigned int jsonLineLength = strlen(WebServer::_jsonLine);

  this->_bridge->wakeup();

  if (this->_relayMessages.length() == 0) {
    const uint8_t size = this->_bridge->size();

    this->_relayMessages.reserve(1+ (size*jsonLineLength) +1); // [ {jsonLine}* ]
    this->_relayMessages = '[';

    for (uint8_t i=0; i<size; ++i) {
      WebServer::_jsonLine[0] = i==0 ? ' ' : ',';
      this->_relayMessages += WebServer::_jsonLine;
    }

    this->_relayMessages += ']';

    yield();
  }

  if (!this->_bridge->prepareRelayList()) {
    return this->_server->send(503);
  }

  uint8_t dec;
  const Bridge::RelayMessage* relay;
  unsigned int ptr = 1;

  while (this->_bridge->nextRelay()) {
    relay = this->_bridge->getCurrentRelayMessage();
    
    if (relay->isOk) {
      dec = relay->relayId /10;
      this->_relayMessages[ptr+6] = dec != 0 ? '0' + dec : ' ';
      this->_relayMessages[ptr+7] = '0' + (relay->relayId - 10* dec);

      dec = relay->pinId /10;
      this->_relayMessages[ptr+25] = dec != 0 ? '0' + dec : ' ';
      this->_relayMessages[ptr+26] = '0' + (relay->pinId - 10* dec);

      this->_relayMessages[ptr+13] = '0' + relay->state;
      this->_relayMessages[ptr+19] = '0' + relay->isLocked;
      this->_relayMessages[ptr+32] = '0' + relay->isNc;
    }

    ptr += jsonLineLength;
  }

  this->_server->send(200, FPSTR(WebServer::TEXT_JSON), this->_relayMessages);
}


void WebServerEsp8266::_writeSerialJson(void) const
{
  if (!this->_server->hasArg(FPSTR(WebServer::PLAIN))) {
    return this->_server->send(400);
  }

  this->_bridge->wakeup();

  JsonArray root;
  {
    DynamicJsonDocument doc(WM_CORE_BUFFER_SIZE);
    String payload = this->_server->arg(FPSTR(WebServer::PLAIN));
    DeserializationError error = deserializeJson(doc, payload, DeserializationOption::NestingLimit(2));
    doc.shrinkToFit();

    root = doc.as<JsonArray>();
  }

  for (JsonObject o : root) {
    const uint8_t relayId = o["i"].as<uint8_t>();
    const uint8_t pinId   = o["p"].as<uint8_t>();
    const bool    isNc    = o["n"].as<bool>();

    this->_bridge->mapRelayToPin(relayId, pinId);
    this->_bridge->isRelayNc(relayId, isNc);
  }

  this->_bridge->save();
}
