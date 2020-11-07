#ifndef WebServer_h
#define WebServer_h



extern "C" {
  #include "c_types.h"
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
#include <ESP8266WebServerSecure.h>
#include "certificate-generated.h"
#endif
#include <ArduinoJson.h>
#include <FS.h>
#include <list>
#include "config.h"
#include "macro.h"
#include "scm-generated.h"
#include "Bridge.h"



class WebServer {

  public:
  void begin(void);
  void loop(void);

  static void setAuthentication(const char* username, const char* password);
  static void setBridge(Bridge* bridge);
  static void setFs(FS &fs);

  protected:
  void _setup(void);

  static const bool _isAllowed(void);
  static void _handleAll(void);
  static void _streamHtml(const char* path, const bool isPublic=true);
  static void _streamJson(const char* path, const char* defaultValue, const bool isPublic=true);
  static void _sendRelayMessage(const Bridge::RelayMessage relay);
  static void _uploadJson(const char* path);
  static void _readSerialJson(void);
  static void _writeSerialJson(void);
  static const size_t _getFileContents(const char* path, char* &buffer);
};



#endif
