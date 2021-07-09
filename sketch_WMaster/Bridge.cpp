#include "Bridge.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



static const uint8_t _length_undefined = 0;




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




Bridge::Bridge(Stream &inputStream)
{
  this->_stream = &inputStream;
  this->_relayMessage = new RelayMessage;
  this->_length = _length_undefined;

  this->_stream->setTimeout(WM_SERIAL_READ_TIMEOUT);
}


uint8_t Bridge::size(void)
{
  if (this->_length == _length_undefined) {
    if (this->_write(String(WM_PATH_SEPARATOR) + String(WM_ACTION_LENGTH))) {
      this->_length = this->_stream->parseInt();
    }
  }

  return this->_length;
}


bool Bridge::getRelay(const uint8_t relayId, const bool unlock) const
{
  String command;
  command.reserve(5);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(unlock ? WM_ACTION_READ_UNLOCK : WM_ACTION_READ);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(relayId);

  return this->_executeOne(command);
}


bool Bridge::setRelay(const uint8_t relayId, const bool state, const bool lock) const
{
  String command;
  command.reserve(7);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(lock ? WM_ACTION_WRITE_LOCK : WM_ACTION_WRITE);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(relayId);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(state);

  return this->_executeOne(command);
}


bool Bridge::mapRelayToPin(const uint8_t relayId, const uint8_t pinId) const
{
  String command;
  command.reserve(8);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(WM_ACTION_MAP);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(relayId);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(pinId);

  return this->_executeOne(command);
}


bool Bridge::isRelayNc(const uint8_t relayId, const bool isNc) const
{
  String command;
  command.reserve(7);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(WM_ACTION_ISNC);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(relayId);
  command.concat(WM_PATH_SEPARATOR);
  command.concat(isNc);

  return this->_executeOne(command);
}


bool Bridge::walkRelayList(TPrintMessageRelayFunction printRelayMessage) const
{
  return this->_executeAll(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_ALL),
    printRelayMessage
  );
}


bool Bridge::prepareRelayList(void) const
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_ALL)
  );
}


bool Bridge::nextRelay(void) const
{
  if (this->_hasMessageEnd()) {
    return false;
  }
  
  return this->_readRelayMessage();
}


bool Bridge::save() const
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_SAVE)
  );
}


bool Bridge::reset() const
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_RESET)
  );
}


bool Bridge::sleep() const
{
  return this->_executeNone(
    String(WM_PATH_SEPARATOR) + String(WM_ACTION_SLEEP)
  );
}


void Bridge::wakeup() const
{
  this->_stream->print(WM_PATH_SEPARATOR);
  this->_stream->flush();
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




bool Bridge::_executeNone(String command) const
{
  return this->_write(command);
}


bool Bridge::_executeOne(String command) const
{
  return this->_write(command)
    && this->_readRelayMessage()
    ;
}


bool Bridge::_executeAll(String command, TPrintMessageRelayFunction printRelayMessage) const
{
  if (this->_write(command)) {
    uint8_t index = 0;

    while (this->_readRelayMessage()) {
      printRelayMessage(this->_relayMessage, index++);
      if (this->_hasMessageEnd()) {
        return true;
      }
    }
  }

  return false;
}

bool Bridge::_write(String command) const
{
  this->_clear();

  LOG(F("command: ")); LOGLN(command);
  uint8_t nbTry = WM_SERIAL_TX_NB_TRY;

  do {
    this->_stream->print(command);
    this->_stream->flush();

    if (this->_timedAvailable()) { 
      if (this->_seekMessageBegin()) {
        return true;
      }
    }

  } while (--nbTry > 0);

  LOGLN(F("timeout"));

  return false;
}


bool Bridge::_hasMessageBegin() const
{
  return this->_stream->peek() == WM_CHAR_RX_BEGIN;
}


bool Bridge::_seekMessageBegin() const
{
  while (this->_stream->available()) {
    if (this->_stream->read() == WM_CHAR_RX_BEGIN) {
      return true;
    }
  }

  return false;
}


bool Bridge::_hasMessageEnd() const
{
  return this->_stream->peek() == WM_CHAR_RX_END;
}


bool Bridge::_seekMessageEnd() const
{
  while (this->_stream->available()) {
    if (this->_stream->read() == WM_CHAR_RX_END) {
      return true;
    }
  }

  return false;
}


bool Bridge::_readRelayMessage() const
{
  /*
    pattern: '^[01] \d{1,3} [01] \d{1,3}$'
    description: "{state} {relay_id} {is_locked} {is_nc} {pin_id}"
  */
  this->_relayMessage->isOk = false;

  if (this->_hasMessageEnd()) {
    return false;
  }
  
  // message min size is: 5 (1int + 1space) = 5*2chars
  if (!this->_timedAvailable(10)) {
    return false;
  }
  
  this->_relayMessage->state    = this->_stream->parseInt();
  this->_relayMessage->relayId  = this->_stream->parseInt();
  this->_relayMessage->isLocked = this->_stream->parseInt();
  this->_relayMessage->isNc     = this->_stream->parseInt();
  this->_relayMessage->pinId    = this->_stream->parseInt();
  
  // .isOk will read/check 2 chars
  if (!this->_timedAvailable(2)) {
    return false;
  }

  this->_relayMessage->isOk     = (WM_DATA_SEPARATOR == this->_stream->read())
    && (WM_LF == this->_stream->read());

  LOG(F("relay: ")); LOGLN(this->_relayMessage->relayId);
  LOG(F("state: ")); LOGLN(this->_relayMessage->state); 
  LOG(F(" isOk: ")); LOGLN(this->_relayMessage->isOk); 

  return true;
}


bool Bridge::_timedAvailable(const int bufferSize) const
{
  const long maxTime = millis() + WM_SERIAL_TX_TIMEOUT;

  do {
    //delay(WM_SERIAL_TX_LOOP_DELAY);
    if (bufferSize <= this->_stream->available()) {
      return true;
    }
  } while (maxTime-millis()>0);

  return false;
}


void Bridge::_clear() const
{
  while (this->_stream->available()) {
    this->_stream->read();
  }
}
