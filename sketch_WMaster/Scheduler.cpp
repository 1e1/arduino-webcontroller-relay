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

  // Wait for the first NTP sync so token expiry and the event window start with a
  // valid clock, but never hang boot forever: retry periodically (the first UDP
  // datagram may be lost) and give up after a few seconds. loop() keeps syncing
  // the clock afterwards, so the calendar recovers once NTP answers.
  constexpr unsigned long NTP_SYNC_TIMEOUT_MS  = 8000;
  constexpr unsigned long NTP_REQUEST_EVERY_MS = 2000;
  const unsigned long startedAt = millis();
  unsigned long lastRequestAt = startedAt;
  while (!_ntp.listenSync()) {
    if (millis() - startedAt >= NTP_SYNC_TIMEOUT_MS) {
      LOGLN("NTP sync timeout, continuing");
      break;
    }
    if (millis() - lastRequestAt >= NTP_REQUEST_EVERY_MS) {
      _ntp.request(WM_NTP_HOST);
      lastRequestAt = millis();
    }
    delay(10);
  }

  const Configuration::GCalendar gCalConfig = _configuration->getGCalendar();
  LOG("refreshToken length: "); LOGLN(gCalConfig.refreshToken.length());

  // Reuse the stored refresh_token to reconnect silently after a reboot;
  // start the device pairing flow only when no valid token is available.
  if (_isValidToken(gCalConfig.refreshToken)) {
    LOGLN("reuse refresh_token");
    _gCalendar.setRefreshToken(gCalConfig.refreshToken);
    _gCalendar.maintain();
  } else {
    LOGLN("start device registration");
    _gCalendar.startQuietRegistration();
  }
}

void Scheduler::loop(void)
{
  _timer1s4mn.update();

  // every 2s: keep the NTP clock fresh (token expiry and the event window rely on it)
  if (_timer1s4mn.isTickBy128()) {
    _ntp.request(WM_NTP_HOST);
  }

  yield();

  if (_ntp.listen()) {
    // Keep the OAuth2 session healthy: refresh the access_token, finish a pending
    // device registration, or bootstrap from the stored refresh_token.
    _gCalendar.maintain();

    if (_gCalendar.isAuthInvalid()) {
      // Google rejected the stored refresh_token (revoked, or expired after the
      // 7-day testing-mode window): forget it and restart the pairing flow.
      LOGLN("refresh_token rejected, re-registering");
      Configuration::GCalendar gCalConfig = _configuration->getGCalendar();
      gCalConfig.refreshToken = "";
      _configuration->setGCalendar(gCalConfig);
      _gCalendar.startQuietRegistration();
    } else if (!_gCalendar.isLinked() && _gCalendar.isAuthenticated()) {
      Configuration::GCalendar gCalConfig = _configuration->getGCalendar();

      // Persist a freshly obtained refresh_token so the next boot reconnects
      // silently (write-once: the token is stable, flash wear stays negligible).
      const String refreshToken = _gCalendar.getRefreshToken();
      if (_isValidToken(refreshToken) && !refreshToken.equals(gCalConfig.refreshToken)) {
        gCalConfig.refreshToken = refreshToken;
        _configuration->setGCalendar(gCalConfig);
        LOGLN("renew g-token");
      }
      _gCalendar.setCalendar(gCalConfig.calendarName);
    }
  }

  yield();

  // every 1mn: sync the calendar and mirror the running events onto the relays
  if (_timer1s4mn.isPureTickBy4()) {
    _ntp.syncRFC3339();
    // Zero-copy: feed the NTP client's internal RFC3339 buffer straight to
    // syncAt(), avoiding a transient String that would fragment the ESP heap.
    _gCalendar.syncAt(_ntp.c_str());
    LOG("* sync @"); LOG(_ntp.c_str()); LOG(" = "); LOG(_ntp.getTimestampUnix()); LOGLN(" *");

    std::list<uint8_t> gRelayIds;
    _assignEventRelayIds(gRelayIds);

    // Switch ON the relays whose event just started (not ON in the previous sync).
    for (uint8_t gRelayId : gRelayIds) {
      if (std::find(_persistentRelayIds.begin(), _persistentRelayIds.end(), gRelayId) == _persistentRelayIds.end()) {
        LOG("welcome: "); LOGLN(gRelayId);
        _bridge->setRelay(gRelayId, true);
      }
    }

    // Switch OFF the relays whose event just ended (gone from the current sync).
    for (uint8_t persistentRelayId : _persistentRelayIds) {
      if (std::find(gRelayIds.begin(), gRelayIds.end(), persistentRelayId) == gRelayIds.end()) {
        LOG("leave: "); LOGLN(persistentRelayId);
        _bridge->setRelay(persistentRelayId, false);
      }
    }

    // Remember the current set for the next iteration.
    _persistentRelayIds.assign(gRelayIds.begin(), gRelayIds.end());
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
