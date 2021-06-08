/*
 * Wright Master Relay
 * 
 * https://github.com/1e1/arduino-webcontroller-relay
 *
 */



#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <list>
#include "config.h"
#include "macro.h"
#include "certificate-generated.h"
#include "Bridge.h"
#include "Configuration.h"
#include "WebServer.h"

#if WM_COMPONENT & WM_COMPONENT_MDNS
  #include <ESP8266mDNS.h>
#endif
#if WM_COMPONENT & WM_COMPONENT_ALEXA
  #include <fauxmoESP.h>
#endif



/** ===================== **/




Configuration::Acl acl;
Bridge* bridge;
WebServer* server;

#if WM_COMPONENT & WM_COMPONENT_ALEXA
struct AlexaRelay {
  const unsigned char alexaId;
  const uint8_t relayId;
};

fauxmoESP fauxmo;
std::list<AlexaRelay> alexaRelayList;
#endif


void setup()
{
  BUSYLED_INIT;
  WAIT(1000);
  LOG_START();
  LOGLN(F("DEBUG ON"));

  LOG(F("reset reason: ")); LOGLN(ESP.getResetReason());
  
  /**
   * init
   */
  WM_SERIAL.begin(WM_SERIAL_SPEED);
  LittleFS.begin();
  bridge = new Bridge(WM_SERIAL);
  Configuration* configuration = new Configuration(LittleFS);

  pinMode(WM_PIN_CONFIG, INPUT_PULLUP);
  pinMode(WM_PIN_SAFEMODE, INPUT_PULLUP);

  bool isUnlocked = digitalRead(WM_PIN_CONFIG) == LOW;
  bool isSafeMode = digitalRead(WM_PIN_SAFEMODE) == LOW;
  bool externalReset = ESP.getResetInfoPtr()->reason == rst_reason::REASON_EXT_SYS_RST;

  LOG(F("isUnlocked=")); LOGLN(isUnlocked);
  LOG(F("isSafeMode=")); LOGLN(isSafeMode);
  LOG(F("externalReset=")); LOGLN(externalReset);

  LOGLN(F("-- load Configuration"));
  configuration->begin();
  configuration->setSafeMode(isSafeMode);

  acl = configuration->getGlobal()->acl;

  if (isUnlocked || externalReset) {
    acl.canAutoRestart = false;
  }
  LOGLN(F("---"));

  WiFi.hostname(certificate::dname);

  if (!acl.isSafeMode) {
    /**
     * set mode Home Assistant
     * (Configuration*)
     */
    BUSYLED_ON;
    LOGLN(F("-- trying to connect to STA:"));
    WiFi.setOutputPower(20); // TODO CONSTANTIZE

    ESP8266WiFiMulti wifiMulti;
    std::list<Configuration::WifiStation> wifiList = configuration->getWifiStationList();
    for (Configuration::WifiStation wifi : wifiList) {
        LOGLN(wifi.ssid);
        wifiMulti.addAP(wifi.ssid.c_str() + '\0', wifi.password.c_str() + '\0');
    }
    
    if (wifiMulti.run(WM_WIFI_CONNEXION_TIMEOUT_MS) == WL_CONNECTED) {
      LOG(F("connected at: ")); LOGLN(WiFi.SSID());
    }
    /* */
    LOGLN(F("---"));
    BUSYLED_OFF;
  }

  std::list<Configuration::Relay> relayList = configuration->getRelayList();

  if (WiFi.status() != WL_CONNECTED) {
    /**
     * set mode Guardian
     * (Configuration*, Configuration::Global*)
     */
    LOGLN(F("-- trying to create AP:"));
    WiFi.setOutputPower(10); // TODO CONSTANTIZE
    LOG(F("AP ssid: "));LOGLN(configuration->getGlobal()->wifiAp.ssid);
    LOG(F("AP password: "));LOGLN(configuration->getGlobal()->wifiAp.password);
    
    IPAddress myIp(192, 168, 240, 1);
    WiFi.softAPConfig(myIp, myIp, IPAddress(255, 255, 255, 0));

    WiFi.mode(WIFI_AP);
    WiFi.softAP(
      configuration->getGlobal()->wifiAp.ssid, 
      configuration->getGlobal()->wifiAp.password, 
      configuration->getGlobal()->wifiAp.channel, 
      configuration->getGlobal()->wifiAp.isHidden
    );
    
    LOGLN(F("---"));
    
    if (!isUnlocked) {
      /**
       * stalk connected devices and switch sensitive relays
       */
      LOGLN(F("trying to detect connected devices"));

      WiFi.setSleepMode(WIFI_LIGHT_SLEEP, 3); // TODO constantize
      delay(WM_WIFI_CONNEXION_TIMEOUT_MS);
      if (WiFi.softAPgetStationNum()) {
        LOGLN(F("OK, run Relay commands:"));
        BUSYLED_ON;

        for (Configuration::Relay relay : relayList) {
          if (relay.onConnect != Configuration::T_indeterminate) {
            LOG(relay.name); LOG(F(" -> ")); LOGLN(relay.onConnect);
            bridge->setRelay(relay.id, static_cast<bool>(relay.onConnect));
          }
        }

        BUSYLED_OFF;
      } else {
        ESP.deepSleepInstant(ESP.deepSleepMax(), WAKE_RF_DISABLED);
      }

      LOGLN(F("---"));
      ESP.restart();
    }

  }

  #if WM_COMPONENT & WM_COMPONENT_MDNS
  LOGLN(F("-- setup mDNS"));
  MDNS.begin(certificate::dname);
  MDNS.addService(WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES ? "https" : "http", "tcp", WM_WEB_PORT);
  LOGLN(F("---"));
  #endif
  
  #if WM_COMPONENT & WM_COMPONENT_ALEXA
  LOGLN(F("-- setup Alexa"));
  fauxmo.createServer(true);
  fauxmo.enable(true);

  LOGLN(F("register devices:"));
  { // register Alexa devices
    for (Configuration::Relay relay : relayList) {
      AlexaRelay alexaRelay {
        .alexaId = fauxmo.addDevice(relay.name.c_str()), 
        .relayId = relay.id,
      };

      LOG(F("- name=\""));
      LOG(relay.name);
      LOG(F("\"; relayId="));
      LOG(alexaRelay.relayId);
      LOG(F("; alexaId="));
      LOG(alexaRelay.alexaId);
      LOGLN();
      
      alexaRelayList.emplace_back(alexaRelay);
    }
  }
  
  LOGLN(F("register callback"));
  fauxmo.onSetState([](unsigned char device_id, const char* device_name, bool state, unsigned char value) {
    LOGF("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
    
    for (AlexaRelay alexaRelay : alexaRelayList) {
      if (alexaRelay.alexaId == device_id) {
        bridge->setRelay(alexaRelay.relayId, state);
        return;
      }
    }
  });
  LOGLN(F("---"));
  #endif

  LOGLN(F("-- setup WebServer"));
  WebServer::setFs(LittleFS);
  WebServer::setBridge(bridge);
  WebServer::setAuthentication(acl.username, acl.password);

  server = new WebServer();
  server->begin();

  LOGLN(F("---"));

  BUSYLED_OFF;
}


unsigned long nextLog = 0;


void loop()
{
  if (acl.canAutoRestart) {
    if (WiFi.status() != WL_CONNECTED) {
      LOGLN(F("** RESTART **"));
      for (uint8_t i=0; i<100; i++) { // TODO REMOVE
        BUSYLED_ON;
        WAIT(1300);
        BUSYLED_OFF;
        WAIT(700);
      }

      ESP.restart();
    }
  }

  if (millis() > nextLog) {
    nextLog += 2000;
    LOGF("[HW] Free heap: %d bytes\n", ESP.getFreeHeap());
  }

  #if WM_COMPONENT & WM_COMPONENT_ALEXA
  fauxmo.handle();
  #endif
  server->loop();
  #if WM_COMPONENT & WM_COMPONENT_MDNS
  MDNS.update();
  #endif


}
