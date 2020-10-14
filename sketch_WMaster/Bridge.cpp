#include "Bridge.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




Bridge::Bridge(Stream &inputStream)
{
  this->_currentStream = &inputStream;
}


const Bridge::RelayMessage Bridge::getRelay(const uint8_t relayId, const bool unlock)
{
  return this->_executeOne(
    String(WM_PATH_SEPARATOR) + String(unlock ? WM_ACTION_READ_UNLOCK : WM_ACTION_READ) + 
    String(WM_PATH_SEPARATOR) + String(relayId)
  );
}


const Bridge::RelayMessage Bridge::setRelay(const uint8_t relayId, const bool state, const bool lock)
{
  return this->_executeOne(
    String(WM_PATH_SEPARATOR) + String(lock ? WM_ACTION_WRITE_LOCK : WM_ACTION_WRITE) + 
    String(WM_PATH_SEPARATOR) + String(relayId) + 
    String(WM_PATH_SEPARATOR) + String(state)
  );
}


const Bridge::RelayMessage Bridge::mapRelayToPin(const uint8_t relayId, const uint8_t pinId)
{
  return this->_executeOne(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_MAP) + 
    String(WM_PATH_SEPARATOR) + String(relayId) + 
    String(WM_PATH_SEPARATOR) + String(pinId)
  );
}


const Bridge::RelayMessage Bridge::isRelayNc(const uint8_t relayId, const bool isNc)
{
  return this->_executeOne(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_ISNC) + 
    String(WM_PATH_SEPARATOR) + String(relayId) + 
    String(WM_PATH_SEPARATOR) + String(isNc)
  );
}


const std::list<Bridge::RelayMessage> Bridge::getRelays()
{
  return this->_executeAll(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_ALL)
  );
}


void Bridge::save()
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_SAVE)
  );
}


void Bridge::reset()
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_RESET)
  );
}


void Bridge::sleep()
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_SLEEP)
  );
}


void Bridge::wakeup()
{
  this->_currentStream->print(WM_PATH_SEPARATOR);
  this->_currentStream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void Bridge::_executeNone(String command)
{
  this->_write(command);
}

const Bridge::RelayMessage Bridge::_executeOne(String command)
{
  if (this->_write(command)) {
    return this->_read();
  }

  return {};
}

const std::list<Bridge::RelayMessage> Bridge::_executeAll(String command)
{
  std::list<Bridge::RelayMessage> relayList;

  if (this->_write(command)) {
    do {
      relayList.emplace_back(this->_read());
    } while(this->_currentStream->available() && !this->_hasMessageEnd());
  }

  return relayList;
}

const bool Bridge::_write(String command)
{
  this->_clear();

  LOG("command: "); LOGLN(command);
  uint8_t nbTry = WM_SERIAL_TX_NB_TRY;

  do {
    this->_currentStream->print(command);
    this->_currentStream->flush();

    int maxTime = WM_SERIAL_TX_TIMEOUT;

    do {
      delay(WM_SERIAL_TX_LOOP_DELAY);
      maxTime -= WM_SERIAL_TX_LOOP_DELAY;

      if (this->_seekMessageBegin()) {
        return true;
      }
    } while (maxTime>0);

  } while (--nbTry > 0);

  return false;
}


const bool Bridge::_hasMessageBegin()
{
  return this->_currentStream->peek() == WM_CHAR_RX_BEGIN;
}


const bool Bridge::_seekMessageBegin()
{
  while (this->_currentStream->available()) {
    if (this->_currentStream->read() == WM_CHAR_RX_BEGIN) {
      return true;
    }
  }

  return false;
}


const bool Bridge::_hasMessageEnd()
{
  return this->_currentStream->peek() == WM_CHAR_RX_END;
}


const bool Bridge::_seekMessageEnd()
{
  while (this->_currentStream->available()) {
    if (this->_currentStream->read() == WM_CHAR_RX_END) {
      return true;
    }
  }

  return false;
}


const Bridge::RelayMessage Bridge::_read()
{
  /*
    pattern: '^[01] \d{1,3} [01] \d{1,3}$'
    description: "{state} {relay_id} {is_locked} {is_nc} {pin_id}"
  */
  Bridge::RelayMessage response {};

  response.isOk = this->_currentStream->read() == WM_CHAR_RX_BEGIN;

  if (response.isOk) {
    response.state    = this->_currentStream->parseInt();
    response.relayId  = this->_currentStream->parseInt();
    response.isLocked = this->_currentStream->parseInt();
    response.isNc     = this->_currentStream->parseInt();
    response.pinId    = this->_currentStream->parseInt();

    response.isOk = this->_currentStream->read() == WM_LF;
  }

  LOG("relay: "); LOGLN(response.relayId);
  LOG("state: "); LOGLN(response.state); 
  LOG(" isOk: "); LOGLN(response.isOk); 

  return response;
}


void Bridge::_clear()
{
  while (this->_currentStream->available()) {
    this->_currentStream->read();
  }
}
