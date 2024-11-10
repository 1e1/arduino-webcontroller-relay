#include "Scheduler.h"







/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/









/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/



void Scheduler::begin(void)
{
  _ntp.begin(WM_NTP_CLIENT_PORT);
  _ntp.request(WM_NTP_HOST);
  do {
      delay(10);
  } while(!_ntp.listenSync());

  Configuration::GCalendar gCalConfig = _configuration->getGCalendar();
  LOG("refreshToken: ");LOGLN(gCalConfig.refreshToken);
  LOG("length: ");LOGLN(gCalConfig.refreshToken.length());

  // prevent "null" value
  if (gCalConfig.refreshToken.length() > 8) {
    LOGLN("setRefreshToken");
    _gCalendar.setRefreshToken(gCalConfig.refreshToken);
    _gCalendar.maintain();
  } else {
    LOGLN("startQuietRegistration");
    _gCalendar.startQuietRegistration();
  }
}

void Scheduler::loop(void)
{
  _timer1s4mn.update();

  // every 2s
  if (_timer1s4mn.isTickBy128()) {
    _ntp.request(WM_NTP_HOST);
  }

  yield();
  
  if (_ntp.listen()) {
    _gCalendar.maintain();

    if (!_gCalendar.isLinked() && _gCalendar.isAuthenticated()) {
      Configuration::GCalendar gCalConfig = _configuration->getGCalendar();
      
      // save refresh_token
      if (_gCalendar.getRefreshToken().length() > 8 && !_gCalendar.getRefreshToken().equals(gCalConfig.refreshToken)) {
        gCalConfig.refreshToken = _gCalendar.getRefreshToken();
        _configuration->setGCalendar(gCalConfig);
        LOGLN("renew g-token");
      }
      _gCalendar.setCalendar(gCalConfig.calendarName);
    }
  }

  yield();

  // every 1mn
  if (_timer1s4mn.isPureTickBy4()) {
    _ntp.syncRFC3339();
    String ts = _ntp.getTimestampRFC3339();
    _gCalendar.syncAt(ts);
    LOG("* new 2mn02s @"); LOG(ts); LOG(" = "); LOG(_ntp.getTimestampUnix()); LOGLN(" *");

    std::list<uint8_t> gRelayIds;
    _assignEventRelayIds(gRelayIds);

    // Iterate through the current events
    for (uint8_t gRelayId : gRelayIds) {
      LOGLN("test-in: " + gRelayId);
      // Check if the event is new (not in the previous list)
      if (std::find(_persitentRelayIds.begin(), _persitentRelayIds.end(), gRelayId) == _persitentRelayIds.end()) {
        // Call welcome() for new events
        LOGLN("welcome: " + gRelayId);
        _bridge->setRelay(gRelayId, true);
      }
    }

    // Iterate through the previous events
    for (uint8_t persitentRelayId : _persitentRelayIds) {
      LOGLN("test-out: " + persitentRelayId);
      // Check if the event is no longer in the current list
      if (std::find(gRelayIds.begin(), gRelayIds.end(), persitentRelayId) == gRelayIds.end()) {
        // Call leave() for events that are no longer in the list
        LOGLN("leave: " + persitentRelayId);
        _bridge->setRelay(persitentRelayId, false);
      }
    }

    // Update the previous list for the next iteration
    _persitentRelayIds.assign(gRelayIds.begin(), gRelayIds.end());
  }
} 



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




const bool Scheduler::_findRelayIdByName(const std::list<Configuration::Relay>& relayList, const String& gEvent, uint8_t& relayId) const
{
  for (const Configuration::Relay& relay : relayList) {
    if (relay.name == gEvent) {
      relayId = relay.id;
      return true;
    }
  }
  return false;
}

void Scheduler::_findRelayIdsByNames(const std::list<Configuration::Relay>& relayList, const std::list<String>& gEvents, std::list<uint8_t>& relayIds) const
{
  uint8_t id;
  for (const String gEvent : gEvents) {
    if (_findRelayIdByName(relayList, gEvent, id)) {
      relayIds.push_back(id);
    } else {
      LOGLN("!!! '" + gEvent + "' not found in the relay names");
    }
  }
}

void Scheduler::_assignEventRelayIds(std::list<uint8_t>& relayIds) const
{
    const std::list<Configuration::Relay> relayList = _configuration->getRelayList();
    const std::list<String> gEvents = _gCalendar.getEventList();

    _findRelayIdsByNames(relayList, gEvents, relayIds);
}
