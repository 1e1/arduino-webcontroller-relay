#include "WebServerASync.h"







/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/









/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




WebServerASync::WebServerASync(FS &fs, Bridge* bridge)
{
  this->setFs(fs);
  this->setBridge(bridge);
  /* * /
  if (isSecure) {
    this->_server = new AsyncWebServer(WM_WEB_PORT_DEFAULT_SECURE);
    _jsonLine[0] = 'S';
  } else {
    this->_server = new AsyncWebServer(WM_WEB_PORT_DEFAULT);
  }
  /* */
  this->_server = new AsyncWebServer(WM_WEB_PORT_DEFAULT);
  /* */
  
  this->_username = new char[1]; // { '\0' };
  this->_password = new char[1]; // { '\0' };
}



void WebServerASync::begin(void)
{
  this->_setRoutes();
  /* * /
  if (_jsonLine[0] == 'S') {
    _jsonLine[0] = ' ';
    this->_server->onSslFileRequest([](void* arg, const char* filename, uint8_t** buf) -> int {
      *buf = 0;
      return 0;
      
      if (filename[0] == 'k') {
          *buf = reinterpret_cast<uint8_t*>(certificate::serverKey);
          return strlen(certificate::serverKey);
      }

      *buf = reinterpret_cast<uint8_t*>(certificate::serverCert);
      return strlen(certificate::serverCert);
    }, NULL);

    //this->_server->beginSecure("c", "k", NULL);
  } else {
    this->_server->begin();
  }
  /* */
  this->_server->begin();
  /* */
}


void WebServerASync::loop(void)
{
}


void WebServerASync::setAuthentication(String username, String password)
{
  this->_username = new char[username.length() + 1];
  strcpy(this->_username, username.c_str());

  this->_password = new char[password.length() + 1];
  strcpy(this->_password, password.c_str());

  LOG("'"); LOG(_username); LOG(F("' : '")); LOG(_password); LOGLN("'");
}

/*
void WebServerASync::setMode(const Mode mode) const
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




void WebServerASync::_setRoutes(void) const
{
  #if WM_COMPONENT & WM_COMPONENT_API
  this->_server->on("/", HTTP_GET, [=](AsyncWebServerRequest* request) {
    request->redirect(WebServer::ROUTE_HOME);
  });

  this->_server->on(WebServer::ROUTE_HOME, HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN("GET " WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
    this->_streamHtml(request, WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
  });
  
  this->_server->on("/api/r", HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN(F("/api/r"));
    this->_streamJson(request, WM_CONFIG_RELAY_PATH, true); 
  });
  #endif

  this->_server->on(WebServer::ROUTE_PORTAL, HTTP_GET, [=](AsyncWebServerRequest* request) {
    if (this->_isAllowed(request)) {
      LOGLN("GET " WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
      this->_streamHtml(request, WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
    }
  });
  
  this->_server->on(WebServer::ROUTE_ABOUT, HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN(F("/about"));
    this->_streamAbout(request);
  });
  
  this->_server->onNotFound([=](AsyncWebServerRequest* request) {
    #if WM_COMPONENT & WM_COMPONENT_API
    switch (request->url().charAt(1)) {
      case 'a':
        LOGLN(F("handle API"));
        return this->_handleApi(request);
      case 'c':
        LOGLN(F("handle Cfg"));
        return this->_handleCfg(request);
    }

    LOGLN(F("handle NotFound"));
    
    this->_handleApi(request);

    #else
    this->_handleCfg();
    #endif
  });
}


void WebServerASync::_setRoutesPortalOnly(void) const
{
  this->_server->on("/", HTTP_GET, [=](AsyncWebServerRequest* request) {
    request->redirect(WebServer::ROUTE_PORTAL);
  });

  this->_server->on(WebServer::ROUTE_PORTAL, HTTP_GET, [=](AsyncWebServerRequest* request) {
    if (this->_isAllowed(request)) {
      LOGLN("GET " WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
      this->_streamHtml(request, WM_WEB_PORTAL_BASENAME "." WM_WEB_FILE_EXT);
    }
  });
  
  this->_server->on(WebServer::ROUTE_ABOUT, HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN(F("/about"));
    this->_streamAbout(request);
  });
  
  this->_server->onNotFound([=](AsyncWebServerRequest* request) {
    LOGLN(F("/notFound"));
    this->_handleCfg(request);
  });
}


void WebServerASync::_setRoutesControllerOnly(void) const
{
  this->_server->on("/", HTTP_GET, [=](AsyncWebServerRequest* request) {
    request->redirect(WebServer::ROUTE_HOME);
  });

  this->_server->on(WebServer::ROUTE_HOME, HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN("GET " WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
    this->_streamHtml(request, WM_WEB_INDEX_BASENAME "." WM_WEB_FILE_EXT);
  });
  
  this->_server->on("/api/r", HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN(F("/api/r"));
    this->_streamJson(request, WM_CONFIG_RELAY_PATH, true); 
  });
  
  this->_server->on(WebServer::ROUTE_ABOUT, HTTP_GET, [=](AsyncWebServerRequest* request) {
    LOGLN(F("/about"));
    this->_streamAbout(request);
  });
  
  this->_server->onNotFound([=](AsyncWebServerRequest* request) {
    LOGLN(F("/notFound"));
    this->_handleApi(request);
  });
}


bool WebServerASync::_isAllowed(AsyncWebServerRequest* request) const
{
  if (this->_username[0] != '\0' && this->_password[0] != '\0') {
    if (!request->authenticate(this->_username, this->_password)) {
      request->requestAuthentication();

      return false;
    }
  }

  return true;
}


void WebServerASync::_handleCfg(AsyncWebServerRequest* request) const
{
  String url = request->url();

  if (!url.startsWith(F("/cfg/")) || !this->_isAllowed(request)) {
    return this->_handleNotFound(request);;
  }

  url.remove(0, String("/cfg/").length());

  if (request->method() == HTTP_DELETE && url.equals(F("reboot"))) {
    request->send(200, FPSTR(WebServer::TEXT_JSON), F("\"reboot\""));
    LOGLN(F("** RESTART **"));
    ESP.restart();
  }

  const char cfg = url.charAt(0);

  switch (cfg) {
    case 'g':
      return this->_uploadAndStreamJson(request, WM_CONFIG_GLOBAL_PATH, false);

    case 'r':
      return this->_uploadAndStreamJson(request, WM_CONFIG_RELAY_PATH, true);

    case 'w':
      return this->_uploadAndStreamJson(request, WM_CONFIG_WIFI_PATH, true);
      
    case 'p':
      return this->_writeAndReadSerial(request);
  }

  this->_handleNotFound(request);
}


void WebServerASync::_handleApi(AsyncWebServerRequest* request) const
{
  String url = request->url();

  if (!url.startsWith(F("/api/r/")) || !this->_isAllowed(request)) {
    return this->_handleNotFound(request);;
  }

  url.remove(0, String("/cfg/").length());
  
  const uint8_t relayId = url.toInt();
    
  switch (request->method()) {
    case HTTP_GET:
      return _bridge->getRelay(relayId)
        ? this->_sendRelayMessage(request, _bridge->getCurrentRelayMessage())
        : request->send(500);
        ;

    case HTTP_PUT:
      if (request->_tempObject != NULL) {
        return request->send(400);
      }

      StaticJsonDocument<WM_API_BUFFER_SIZE> jsonBuffer;
      DeserializationError error = deserializeJson(jsonBuffer, (uint8_t*)(request->_tempObject), DeserializationOption::NestingLimit(2));
      if (!error) {
        const bool state = jsonBuffer["s"].as<bool>();

        if (_bridge->setRelay(relayId, state)) {
          return this->_sendRelayMessage(request, _bridge->getCurrentRelayMessage());
        }
      }

      return request->send(500);
  }

  this->_handleNotFound(request);
}


void WebServerASync::_handleNotFound(AsyncWebServerRequest* request) const
{
  AsyncWebServerResponse* response = request->beginResponse(404);
  response->addHeader(FPSTR(WebServer::CACHE_CONTROL), FPSTR(WebServer::MAX_AGE_86400));

  request->send(response);
}


void WebServerASync::_uploadAndStreamJson(AsyncWebServerRequest* request, const char* path, const bool isArray) const
{
  if (request->method() == HTTP_POST) {
    if (!this->_uploadJson(request, path)) {
      return request->send(400);
    }
  }

  this->_streamJson(request, path, isArray);
}


void WebServerASync::_writeAndReadSerial(AsyncWebServerRequest* request) const
{
  if (request->method() == HTTP_POST) {
    if (!this->_writeSerialJson(request)) {
      return request->send(400);
    }
  }
  
  this->_readSerialJson(request);
}


void WebServerASync::_streamAbout(AsyncWebServerRequest* request) const
{
  AsyncWebServerResponse* response = request->beginResponse(
    200, 
    FPSTR(WebServer::TEXT_JSON), 
    F("{\"repo\":\"" SCM_REPO "\",\"hash\":\"" SCM_HASH "\",\"date\":\"" SCM_DATE "\",\"chan\":\"" SCM_CHAN "\"}")
  );

  response->addHeader(FPSTR(WebServer::X_HEAP), String(ESP.getFreeHeap()));
  response->addHeader(FPSTR(WebServer::X_UPTIME), String(millis()));

  request->send(response);
}


void WebServerASync::_streamHtml(AsyncWebServerRequest* request, const char* path) const
{
  AsyncWebServerResponse *response = request->beginResponse(*(this->_fs), path, FPSTR(WebServer::TEXT_HTML));
  
  if (String(WM_WEB_FILE_EXT_GZ).equals(WM_WEB_FILE_EXT)) {
    response->addHeader(FPSTR(WebServer::CONTENT_ENCODING), F("gzip"));
  }
  if (String(WM_WEB_FILE_EXT_BR).equals(WM_WEB_FILE_EXT)) {
    response->addHeader(FPSTR(WebServer::CONTENT_ENCODING), F("br"));
  }

  response->addHeader(FPSTR(WebServer::CACHE_CONTROL), FPSTR(WebServer::MAX_AGE_86400));
  request->send(response);
}


void WebServerASync::_streamJson(AsyncWebServerRequest* request, const char* path, const bool isArray) const
{
  if (!this->_fs->exists(path)) {
    return request->send(200, FPSTR(WebServer::TEXT_JSON), isArray ? F("[]") : F("{}"));
  }

  request->send(*(this->_fs), path, FPSTR(WebServer::TEXT_JSON));
}


void WebServerASync::_sendRelayMessage(AsyncWebServerRequest* request, const Bridge::RelayMessage* relay) const
{
  if (!relay->isOk) {
    return request->send(500);
  }

  request->send(200, FPSTR(WebServer::TEXT_JSON), relay->state ? FPSTR(WebServer::JSON_ARRAY_EMPTY) : FPSTR(WebServer::JSON_OBJECT_EMPTY));
}


bool WebServerASync::_uploadJson(AsyncWebServerRequest* request, const char* path) const
{
  if (request->_tempObject != NULL) {
    DynamicJsonDocument jsonBuffer(WM_CONFIG_BUFFER_SIZE);
    DeserializationError error = deserializeJson(jsonBuffer, (uint8_t*)(request->_tempObject), DeserializationOption::NestingLimit(2));
    
    if (!error) {
      File file = this->_fs->open(path, "w");
      serializeJson(jsonBuffer, file);
      file.close();

      return true;
    }
  }

  return false;
}


void WebServerASync::_readSerialJson(AsyncWebServerRequest* request) const
{
  this->_bridge->wakeup();

  WebServer::_jsonLine[0] = ' ';

  if (!this->_bridge->prepareRelayList()) {
    return request->send(503);
  }

  AsyncResponseStream* response = request->beginResponseStream(FPSTR(WebServer::TEXT_JSON));
  response->print('[');

  uint8_t dec;
  const Bridge::RelayMessage* relay;

  while (this->_bridge->nextRelay()) {
    relay = this->_bridge->getCurrentRelayMessage();

    if (relay->isOk) {
      dec = relay->relayId /10;
      WebServer::_jsonLine[6] = dec != 0 ? '0' + dec : ' ';
      WebServer::_jsonLine[7] = '0' + (relay->relayId - 10* dec);

      WebServer::_jsonLine[13] = '0' + relay->state;
      WebServer::_jsonLine[19] = '0' + relay->isLocked;

      dec = relay->pinId /10;
      WebServer::_jsonLine[25] = dec != 0 ? '0' + dec : ' ';
      WebServer::_jsonLine[26] = '0' + (relay->pinId - 10* dec);

      WebServer::_jsonLine[32] = '0' + relay->isNc;

      response->print(WebServer::_jsonLine);

      if (WebServer::_jsonLine[0] == ' ') {
        WebServer::_jsonLine[0] = ',';
      }
    }

    response->print(")");
  }

  response->print(']');
  request->send(response);
}


bool WebServerASync::_writeSerialJson(AsyncWebServerRequest* request) const
{
  if (request->_tempObject != NULL) {
    return false;
  }

  this->_bridge->wakeup();

  JsonArray root;
  {
    DynamicJsonDocument jsonBuffer(WM_CORE_BUFFER_SIZE);
    DeserializationError error = deserializeJson(jsonBuffer, (uint8_t*)(request->_tempObject), DeserializationOption::NestingLimit(2));
    if (error) {
      return false;
    }

    jsonBuffer.shrinkToFit();

    root = jsonBuffer.as<JsonArray>();
  }

  for (JsonObject o : root) {
    const uint8_t relayId = o["i"].as<uint8_t>();
    const uint8_t pinId   = o["p"].as<uint8_t>();
    const bool    isNc    = o["n"].as<bool>();

    this->_bridge->mapRelayToPin(relayId, pinId);
    this->_bridge->isRelayNc(relayId, isNc);
  }

  this->_bridge->save();

  return true;
}
