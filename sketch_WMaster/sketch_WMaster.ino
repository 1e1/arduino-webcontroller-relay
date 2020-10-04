/*
 * Wright Master Relay
 * 
 * https://github.com/1e1/arduino-webcontroller-relay
 *
 */



#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <ArduinoJson.h>
#include <Vector.h>
#include "config.h"
#include "macro.h"
#include "Configuration.h"



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
    * EXEC(cmd)
    * ------
    * do
    *  Serial.print(cmd)
    * while (Serial.read() == '?')
    * return Serial.read()
    * 
    * 
    * PORTAL:
    * -------
    * GET /portal
    * GET /cfg/mac
    * GET /cfg/reset
    * GET /cfg/{cat}
    * POST /cfg/{cat} {object}
    * if time > lockedTime && BASIC_AUTH
    * then
    *   portal:
    *   form: BASIC_AUTH - password[8-63] - save
    *   form: AP - ssid[0-32] - password[8-63] - save
    *   form: - reboot
    *   wifis:
    *   form: ssid1[0-32] - *********[8-63] - delete
    *   form: ssid2[0-32] - *********[8-63] - delete
    *   form: ssid3[0-32] - password3[8-63] - add
    *   devices:
    *   form: MAC1[17] - deviceName1[0-32] - command[0-8] - delete
    *   form: MAC2[17] - deviceName2[0-32] - command[0-8] - add
    *   relays:
    *   form: 0[2] - NC[1] - name0[0-32] - delete
    *   form: 5[2] - NO[1] - name5[0-32] - delete
    *   form: 42[2] - NC[1] - name42[0-32] - add
    * end
    * lockedTime = time + 2s
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




ESP8266WebServer server(80); // TODO constantize
bool canAutoRestart = false;


void send(const char* command)
{
  do {
    Serial.print(command);
    Serial.flush();

    while (!Serial.available());

  } while (Serial.peek() == '?'); // TODO constantize
}


void setup()
{
  BUSYLED_WORK;
  Serial.begin(WM_USB_SPEED);
  LOGLN(F("DEBUG ON"));
  
  /**
   * init
   */
  Configuration* configuration = new Configuration();

  pinMode(WM_PIN_ERASE, INPUT_PULLUP);
  pinMode(WM_PIN_CONFIG, INPUT_PULLUP);

  if (digitalRead(WM_PIN_ERASE) == LOW) {
    configuration->erase();
  }

  bool isUnlocked = digitalRead(WM_PIN_CONFIG) == LOW);

  configuration->begin();

  if (!isUnlocked) {
    canAutoRestart = configuration->getGlobal()->autoRestart;
  }

  if (configuration->exists()) {
    /**
     * set mode Home Assistant
     * (Configuration*)
     */
    WiFi.mode(WIFI_STA);
    Vector<Configuration::Wifi> wifis = configuration->getWifiList();
    for (Configuration::Wifi wifi : wifis) {
        WiFi.begin(wifi.ssid, wifi.password);
        WiFi.waitForConnectResult(30000); // TODO constantize
        
        if (WiFi.status() == WL_CONNECTED) {
          break;
        }
    }
  }

  WiFi.setSleepMode(WIFI_LIGHT_SLEEP, 3); // TODO constantize

  if (WiFi.status() != WL_CONNECTED) {
    /**
     * set mode Guardian
     * (Configuration*, Configuration::Global*)
     */
    WiFi.mode(WIFI_AP);
    WiFi.softAP(
      configuration->getGlobal()->wifiSsid, 
      configuration->getGlobal()->wifiPassword, 
      configuration->getGlobal()->wifiChannel, 
      configuration->getGlobal()->wifiIsHidden
    );
    
    if (!isUnlocked) {
      /**
       * stalk connected devices and exec their relative command
       */
      delay(30000);
      Vector<Configuration::Device> devices = configuration->getDeviceList(); 
      struct station_info* station_list = wifi_softap_get_station_info();
      while (station_list != NULL) {
        char station_mac[18] = {0}; 
        sprintf(station_mac, "%02X:%02X:%02X:%02X:%02X:%02X", MAC2STR(station_list->bssid));
        // match
        for (Configuration::Device d : devices) {
          if (memcmp(d.mac, station_mac, 18) == 0) {
            send(d.command);
          }
        }

        station_list = STAILQ_NEXT(station_list, next);
      }
    }

    wifi_softap_free_station_info();
    ESP.deepSleepInstant(30E6, WAKE_RF_DISABLED);
  }
}


void loop()
{
  if (canAutoRestart) {
    if (WiFi.status() != WL_CONNECTED) {
      ESP.restart();
    }
  }

  LOGLN("waiting");
  WAIT(1000);
}
