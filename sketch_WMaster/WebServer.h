#ifndef WebServer_h
#define WebServer_h



extern "C" {
  #include "c_types.h"
}

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266WebServerSecure.h>
#include <ArduinoJson.h>
#include <FS.h>
#include <list>
#include "config.h"
#include "macro.h"
#include "certificate-generated.h"
#include "Bridge.h"



class WebServer {

  public:
  void begin();
  void loop();

  static void setAuthentication(const char* username, const char* password);
  static void setBridge(Bridge* bridge);
  static void setFs(FS &fs);

  protected:
  void _setup();

  static const bool _isAllowed();
  static void _handleAll();
  static void _streamBrotli(const char* path, const bool isPublic=true);
  static void _streamJson(const char* path, const char* defaultValue, const bool isPublic=true);
  static void _sendRelayMessage(const Bridge::RelayMessage relay);
  static void _uploadJson(const char* path);
  static void _readSerialJson();
  static void _writeSerialJson();
  static const size_t _getFileContents(const char* path, char* &buffer);
};



#endif
