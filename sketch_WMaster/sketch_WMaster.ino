/*
 * Wright Master Relay
 * 
 * https://github.com/1e1/arduino-webcontroller-relay
 *
 */



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <Vector.h>
#include <time.h>
#include <sys/time.h>
#include "config.h"
#include "macro.h"
#include "certificate-generated.h"
#include "Bridge.h"
#include "Configuration.h"
#include "WebServer.h"



/** ===================== **/

  /*
    * SETUP()
    * -------
    * if pinX == GROUND
    * then config_erase()
    * end
    * 
    * if config_exists()
    * then
    *   for credential in config[wifis]
    *     nbRetry = 10
    *     do
    *       WiFi.connect(credential[ssid], credential[password])
    *       idle(500ms)
    *     while (!WL_CONNECTED && --nbRetry)
    *   end
    * end
    * 
    * if !WL_CONNECTED
    * then
    *   softAP(config[ap_ssid], config[ap_password])
    *   if pinY == GROUND
    *   then
    *   else
    *     idle(30s)
    *     stalk(&mac)
    *     for device in config[devices]
    *       if mac == device.mac
    *       then
    *         EXEC(device.command)
    *       end
    *     end
    *     sleep(30s)
    *   end
    * end
    * 
    * 
    * LOOP()
    * ------
    * if WL_CONNECTED
    *   then
    *     PORTAL(form, save, reboot):8443
    *     WEBAPP:8080
    *     FAUXMO:80 if Alexa
    *     sleep(2s)
    *   else ESP.reset()
    * end
    * 
    * 
    * WEBAPP:
    * -------
    * GET /
    * GET /api/{relayId}
    * POST /api/{relayId} {pin?=int, isNc?=bool, value=bool}
    * PUT /api/{relayId}/{value=bool}
    * POST /api/special {action="save"}
    * relays = EXEC(/$)
    * for relay in relays
    *   displayHtml(relay)
    * 
    * 
    */




Configuration::Acl acl;
WebServer* server;


void setup()
{
  BUSYLED_WORK;
  Serial.begin(WM_SERIAL_SPEED);
  WAIT(1000);
  LOGLN(PSTR("DEBUG ON"));

  LOG("reset reason: "); LOGLN(ESP.getResetReason());
  
  /**
   * init
   */
  LittleFS.begin();
  Bridge* bridge = new Bridge(WM_SERIAL);
  Configuration* configuration = new Configuration(LittleFS);

  pinMode(WM_PIN_CONFIG, INPUT_PULLUP);
  pinMode(WM_PIN_SAFEMODE, INPUT_PULLUP);

  bool isUnlocked = digitalRead(WM_PIN_CONFIG) == LOW;
  bool isSafeMode = digitalRead(WM_PIN_SAFEMODE) == LOW;
  bool externalReset = ESP.getResetInfoPtr()->reason == rst_reason::REASON_EXT_SYS_RST;

  LOGLN(PSTR("-- load Configuration"));
  configuration->begin();
  configuration->setSafeMode(isSafeMode);

  acl = configuration->getGlobal()->acl;

  if (isUnlocked) {
    acl.canAutoRestart = false;
  }
  LOGLN(PSTR("---"));

  if (!acl.isSafeMode && !externalReset) {
    /**
     * set mode Home Assistant
     * (Configuration*)
     */
    BUSYLED_IDLE;
    LOGLN(PSTR("-- trying to connect to STA:"));

    WiFi.mode(WIFI_STA);
    std::list<Configuration::WifiStation> wifiList = configuration->getWifiStationList();
    for (Configuration::WifiStation wifi : wifiList) {
        LOGLN(wifi.ssid);
        WiFi.begin(wifi.ssid, wifi.password);
        
        if (WiFi.waitForConnectResult(30000) == WL_CONNECTED) { // TODO constantize
          break;
        }
    }

    LOGLN(PSTR("---"));
  }

  //WiFi.setSleepMode(WIFI_LIGHT_SLEEP, 3); // TODO constantize

  if (WiFi.status() != WL_CONNECTED) {
    /**
     * set mode Guardian
     * (Configuration*, Configuration::Global*)
     */
    LOGLN(PSTR("-- trying to create AP:"));
    LOG("AP ssid: ");LOGLN(configuration->getGlobal()->wifiAp.ssid);
    LOG("AP password: ");LOGLN(configuration->getGlobal()->wifiAp.password);
    
    WiFi.mode(WIFI_AP);
    WiFi.softAP(
      configuration->getGlobal()->wifiAp.ssid, 
      configuration->getGlobal()->wifiAp.password, 
      configuration->getGlobal()->wifiAp.channel, 
      configuration->getGlobal()->wifiAp.isHidden
    );
    
    LOGLN(PSTR("---"));
    
    if (!isUnlocked) {
      /**
       * stalk connected devices and switch sensitive relays
       */
      BUSYLED_IDLE;
      LOGLN(PSTR("trying to detect connected devices"));

      delay(30); // TODO 30s = 30000
      if (WiFi.softAPgetStationNum()) {
        LOGLN(PSTR("OK, run Relay commands:"));

        std::list<Configuration::Relay> relayList = configuration->getRelayList();
        for (Configuration::Relay relay : relayList) {
          if (relay.onConnect != Configuration::T_indeterminate) {
            LOG(relay.name); LOG(" -> "); LOGLN(relay.onConnect);
            bridge->setRelay(relay.id, static_cast<bool>(relay.onConnect));
          }
        }
      }

      LOGLN(PSTR("---"));
    }
  }

  LOGLN(PSTR("-- setup WebServer"));
  WebServer::setFs(LittleFS);
  WebServer::setBridge(bridge);
  WebServer::setAuthentication(acl.username, acl.password);

  server = new WebServer();
  server->begin();
  
  LOGLN(PSTR("---"));

  LOGLN(PSTR("-- setup mDNS"));
  MDNS.begin(certificate::dname);
  MDNS.addService("https", "tcp", WM_WEB_PORT);
  LOGLN(PSTR("---"));

  BUSYLED_NONE;
}


void loop()
{
  if (acl.canAutoRestart) {
    if (WiFi.status() != WL_CONNECTED) {
      LOGLN(PSTR("** RESTART **"));

      ESP.deepSleepInstant(30E6, WAKE_RF_DISABLED);
      ESP.restart();
    }
  }

  server->loop();
  MDNS.update();
}
