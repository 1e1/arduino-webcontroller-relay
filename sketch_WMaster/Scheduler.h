#ifndef Scheduler_H_
#define Scheduler_H_


#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiClientSecureBearSSL.h>
#include <WiFiUdp.h>
#include <FastTimer.hpp>
#include <GoogleSchedular.hpp>

#include <functional>

#include "Bridge.h"
#include "Configuration.h"



using TGetStateCallback = std::function<void(unsigned char, const char*, bool, unsigned char)>;



class Scheduler {

    public:
    Scheduler(Configuration* cfg, Bridge* bdg)
    : _configuration(cfg), _bridge(bdg), _gCalendar(WS_GOOGLE_API_CLIENT_ID, WS_GOOGLE_API_CLIENT_SECRET, &_ntp)
    {}

    __attribute__((always_inline)) inline GoogleSchedular* getGCalendar(void) { return &_gCalendar; }

    void begin(void);
    void loop(void);


    protected:

    // A refresh_token shorter than this is treated as absent ("", "null", ...).
    static constexpr size_t MIN_REFRESH_TOKEN_LENGTH = 8;
    static bool _isValidToken(const String& token) { return token.length() > MIN_REFRESH_TOKEN_LENGTH; }

    const bool _findRelayIdByName(const std::list<Configuration::Relay>& relayList, const String& gEvent, uint8_t& relayId) const;
    void _findRelayIdsByNames(const std::list<Configuration::Relay>& relayList, const std::list<String>& gEvents, std::list<uint8_t>& relayIds) const;
    void _assignEventRelayIds(std::list<uint8_t>& relayIds) const;

    Bridge* _bridge = nullptr;
    Configuration* _configuration = nullptr;
    TimestampNtp<WiFiUDP> _ntp;
    GoogleSchedular _gCalendar;
    FastTimer<FastTimerPrecision::P_1s_4m> _timer1s4mn;
    // Relays currently ON because of a running calendar event, so the next sync
    // can tell which events just started and which just ended.
    std::list<uint8_t> _persistentRelayIds;
};


#endif
