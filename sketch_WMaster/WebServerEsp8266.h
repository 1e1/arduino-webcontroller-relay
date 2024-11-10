#ifndef WebServerEsp8266_H_
#define WebServerEsp8266_H_



extern "C" {
  #include "c_types.h"
}

#include <ESP8266WebServer.h>
#if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
#include <ESP8266WebServerSecure.h>
#endif
#include <ArduinoJson.h>
#include "WebServer.h"

#if WM_COMPONENT & WM_COMPONENT_GCALENDAR
#include "GoogleSchedular.hpp"
#endif



class WebServerEsp8266 : public WebServer {

  public:
  WebServerEsp8266(FS* fs, Bridge* bridge);

  #if WM_COMPONENT & WM_COMPONENT_GCALENDAR
  WebServerEsp8266(FS* fs, Bridge* bridge, GoogleSchedular* gCalendar)
  : WebServerEsp8266(fs, bridge)
  { this->_gCalendar = gCalendar; };
  #endif

  void begin(void);
  void loop(void);

  void setAuthentication(String username, String password);
  

  protected:
  void _init(void);
  void _setServer(void);
  void _setRoutes(void);
  const bool _isAllowed(void) const;
  void _handleCfg(void);
  void _handleApi(void) const;
  void _handleOa2(void) const;
  void _uploadAndStreamJson(const char* path, const bool isArray=false, const bool isUpload=false) const;
  void _redirect(String uri) const;
  void _send404(void) const;
  void _streamAbout(void) const;
  void _streamFile(const char* path, const char* contentType) const;
  void _streamHtml(const char* path) const;
  void _streamJson(const char* path, const bool isArray=false) const;
  void _sendRelayMessage(const Bridge::RelayMessage* relay) const;
  void _uploadJson(const char* path) const;
  void _readSerialJson(void);
  void _writeSerialJson(void) const;

  char* _username;
  char* _password;

  #if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
  // ESP8266WebServerTemplate<WiFiServerSecure>* _server;
  BearSSL::ESP8266WebServerSecure* _server;
  #else
  // ESP8266WebServerTemplate<WiFiServer>* _server;
  ESP8266WebServer* _server;
  #endif

  String _relayMessages;

  #if WM_COMPONENT & WM_COMPONENT_GCALENDAR
  GoogleSchedular* _gCalendar;
  #endif
};



#endif
