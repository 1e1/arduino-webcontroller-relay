#ifndef WebServer_H_
#define WebServer_H_


#include <FS.h>
#include "certificate-generated.h"
#include "config.h"
#include "macro.h"
#include "scm-generated.h"
#include "Bridge.h"



class WebServer {

  public:
  typedef enum { 
      MODE_NONE, 
      MODE_PORTAL, 
      MODE_API,
      MODE_ALL, 
  } Mode;

  static constexpr const char TEXT_HTML[] PROGMEM = "text/html";
  static constexpr const char TEXT_JSON[] PROGMEM = "text/json";
  static constexpr const char PLAIN[] PROGMEM = "plain";
  static constexpr const char LOCATION[] PROGMEM = "Location";
  static constexpr const char CONTENT_ENCODING[] PROGMEM = "Content-Encoding";
  static constexpr const char CACHE_CONTROL[] PROGMEM = "Cache-Control";
  static constexpr const char X_HEAP[] PROGMEM = "X-Heap";
  static constexpr const char X_UPTIME[] PROGMEM = "X-Uptime";
  static constexpr const char MAX_AGE_86400[] PROGMEM = "max-age=86400";
  static constexpr const char JSON_OBJECT_EMPTY[] PROGMEM = "{}";
  static constexpr const char JSON_ARRAY_EMPTY[] PROGMEM = "[]";

  static constexpr const char ROUTE_HOME[] = "/home";
  static constexpr const char ROUTE_PORTAL[] = "/portal";
  static constexpr const char ROUTE_ABOUT[] = "/about";

  virtual void begin(void) =0;
  virtual void loop(void) =0;

  void setFs(FS &fs);
  void setBridge(Bridge* bridge);
  virtual void setAuthentication(String username, String password) =0;
  virtual void setMode(const Mode mode) =0;

  protected:

  static char _jsonLine[];

  Bridge* _bridge;
  FS* _fs;
};



#endif
