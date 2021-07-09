#ifndef WebServerASync_H_
#define WebServerASync_H_



#if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
  #ifndef ASYNC_TCP_SSL_ENABLED
    #define ASYNC_TCP_SSL_ENABLED   1
  #endif
#endif


extern "C" {
  #include "c_types.h"
}

#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
#include "WebServer.h"



class WebServerASync : public WebServer {

  public:
  WebServerASync(FS &fs, Bridge* bridge);

  void begin(void);
  void loop(void);

  void setAuthentication(String username, String password);
  void setMode(const Mode mode) const;

  protected:
  void _setRoutes(void) const;
  void _setRoutesPortalOnly(void) const;
  void _setRoutesControllerOnly(void) const;
  bool _isAllowed(AsyncWebServerRequest* request) const;
  void _handleCfg(AsyncWebServerRequest* request) const;
  void _handleApi(AsyncWebServerRequest* request) const;
  void _handleNotFound(AsyncWebServerRequest* request) const;
  void _uploadAndStreamJson(AsyncWebServerRequest* request, const char* path, const bool isArray=false) const;
  void _writeAndReadSerial(AsyncWebServerRequest* request) const;
  void _streamAbout(AsyncWebServerRequest* request) const;
  void _streamHtml(AsyncWebServerRequest* request, const char* path) const;
  void _streamJson(AsyncWebServerRequest* request, const char* path, const bool isArray=false) const;
  void _sendRelayMessage(AsyncWebServerRequest* request, const Bridge::RelayMessage* relay) const;
  bool _uploadJson(AsyncWebServerRequest* request, const char* path) const;
  void _readSerialJson(AsyncWebServerRequest* request) const;
  bool _writeSerialJson(AsyncWebServerRequest* request) const;

  char* _username;
  char* _password;

  AsyncWebServer* _server;
};



#endif
