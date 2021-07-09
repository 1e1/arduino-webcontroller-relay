/*
 * Wright Master Relay
 * 
 * https://github.com/1e1/arduino-webcontroller-relay
 *
 */



#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ArduinoJson.h>
#include <LittleFS.h>
#include <list>
#include "certificate-generated.h"
#include "config.h"
#include "macro.h"
#include "Bridge.h"
#include "Configuration.h"
#ifdef WM_USE_ASYNC
#include "WebServerASync.h"
#else
#include "WebServerEsp8266.h"
#endif

#if WM_COMPONENT & WM_COMPONENT_MDNS
#include <ESP8266mDNS.h>
#endif
#if WM_COMPONENT & WM_COMPONENT_ALEXA
#include <fauxmoESP.h>
#endif

#ifdef LED_BUILTIN
#include "MorseLed.hpp"
#endif



/** ===================== **/




Configuration::Acl acl;
Bridge* bridge;
WebServer* server;
bool isAdminReset;

#if WM_COMPONENT & WM_COMPONENT_ALEXA
fauxmoESP fauxmo;
#endif


#if WM_LOG_LEVEL != WM_LOG_LEVEL_OFF
unsigned long nextLog = 0;
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
  pinMode(WM_WAKE_UP_PIN, INPUT_PULLUP);

  const bool isSafeMode = digitalRead(WM_PIN_SAFEMODE) == LOW;
  const bool isUnlocked = digitalRead(WM_PIN_CONFIG) == LOW;
  const bool externalReset = ESP.getResetInfoPtr()->reason == rst_reason::REASON_EXT_SYS_RST;

  isAdminReset = isUnlocked || externalReset;

  LOG(F("isUnlocked=")); LOGLN(isUnlocked);
  LOG(F("isSafeMode=")); LOGLN(isSafeMode);
  LOG(F("externalReset=")); LOGLN(externalReset);

  #if WM_COMPONENT & WM_COMPONENT_LED
  {
    LOGLN(F("-- init Morse Led"));
    MorseLed led(LED_BUILTIN);

    if (isUnlocked) {
      led.print("free");
      led.space();
    }

    if (isSafeMode) {
      led.print("safe");
      led.space();
    }

    if (externalReset) {
      led.print("reset");
      led.space();
    }
  }
  #endif

  LOGLN(F("-- load Configuration"));
  configuration->begin();
  configuration->setSafeMode(isSafeMode);

  acl = configuration->getGlobal()->acl;

  if (isAdminReset) {
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
    
    //IPAddress myIp(192, 168, 0, 1); // TODO CONSTANTIZE
    //WiFi.softAPConfig(myIp, myIp, IPAddress(255, 255, 255, 0));

    WiFi.mode(WIFI_AP);
    WiFi.softAP(
      configuration->getGlobal()->wifiAp.ssid, 
      configuration->getGlobal()->wifiAp.password, 
      configuration->getGlobal()->wifiAp.channel, 
      configuration->getGlobal()->wifiAp.isHidden
    );
    
    LOGLN(F("---"));
    
    if (!isAdminReset) {
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
        //ESP.deepSleepInstant(ESP.deepSleepMax(), WAKE_RF_DISABLED); // TODO constantize (microseconds)
        delay(5 *60 *1000); // TODO constantize (millis)
      }

      LOGLN(F("---"));
      ESP.restart();
    }

  }

  #if WM_COMPONENT & WM_COMPONENT_MDNS
  LOGLN(F("-- setup mDNS"));
  MDNS.begin(certificate::dname);
  #if WM_WEB_SERVER_SECURE == WM_WEB_SERVER_SECURE_YES
  MDNS.addService(F("https"), F("tcp"), WM_WEB_PORT_DEFAULT_SECURE);
  #else
  MDNS.addService(F("http"), F("tcp"), WM_WEB_PORT_DEFAULT);
  #endif
  LOGLN(F("---"));
  #endif

  if (isAdminReset) {
    LOGLN(F("-- setup WebServer (portal)"));
    #ifdef WM_USE_ASYNC
    server = new WebServerASync(LittleFS, bridge);
    #else
    server = new WebServerEsp8266(LittleFS, bridge);
    #endif
    server->setAuthentication(acl.username, acl.password);
    server->begin();
    LOGLN(F("---"));
  } else {
    #if WM_COMPONENT & WM_COMPONENT_ALEXA
    LOGLN(F("-- setup Alexa"));
    fauxmo.createServer(true);
    fauxmo.enable(true);

    LOGLN(F("register devices:"));
    { // register Alexa devices
      for (Configuration::Relay relay : relayList) {
        LOG(relay.id);
        LOG(F("- name=\""));
        LOG(relay.name);
        LOGLN('"');

        fauxmo.addDevice(relay.name.c_str());
      }
    }
    
    LOGLN(F("register callback"));
    fauxmo.onSetState([=](unsigned char device_id, const char* device_name, bool state, unsigned char value) {
      LOGF("[MAIN] Device #%d (%s) state: %s value: %d\n", device_id, device_name, state ? "ON" : "OFF", value);
      
      for (Configuration::Relay relay : relayList) {
        if (relay.name.equals(device_name)) {
          bridge->setRelay(relay.id, state);
          return;
        }
      }
    });

    LOGLN(F("---"));
    #endif

    LOGLN(F("-- setup WebServer (api)"));
    #ifdef WM_USE_ASYNC
    server = new WebServerASync(LittleFS, bridge);
    #else
    server = new WebServerEsp8266(LittleFS, bridge);
    #endif
    server->setAuthentication(acl.username, acl.password);
    server->begin();
    LOGLN(F("---"));
  }

  BUSYLED_OFF;
  #if WM_LOG_LEVEL != WM_LOG_LEVEL_OFF
  nextLog = millis();
  #endif
}


void loop()
{
  if (acl.canAutoRestart) {
    if (WiFi.status() != WL_CONNECTED) {
      LOGLN(F("** WiFi disconnected **"));
      LOGLN(F("** RESTART **"));
      ESP.restart();
    }
  }

  if (!isAdminReset) {
    if (digitalRead(WM_PIN_CONFIG) == LOW) {
      LOGLN(F("** Unlock pressed **"));
      LOGLN(F("** RESTART **"));
      ESP.restart();
    }
  }

  #if WM_LOG_LEVEL != WM_LOG_LEVEL_OFF
  if (millis() > nextLog) {
    nextLog += 2000;
    LOGF("[HW] Free heap: %d bytes\n", ESP.getFreeHeap());
  }
  #endif

  #if WM_COMPONENT & WM_COMPONENT_ALEXA
  fauxmo.handle();
  yield();
  #endif
  server->loop();
  #if WM_COMPONENT & WM_COMPONENT_MDNS
  yield();
  MDNS.update();
  #endif


}
