#ifndef AbstractStream_H_
#define AbstractStream_H_



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macro.h"
#include "scm-generated.h"
#include "AbstractInterface.h"
#include "PowerManager.h"
#include "Relay.h"




class AbstractStream : public AbstractInterface {

  public:
  const bool read(void);
  void process(void);
  void terminate(void) const;

  // inline
  __attribute__((always_inline)) inline void setStream(Stream* inputStream) { this->_currentStream = inputStream; };

  protected:
  void _printData(const uint8_t data) const;
  void _printOne(const uint8_t relay) const;
  void _printAbout(void) const;
  void _printLength(void) const;
  void _printAll(void) const;

  bool _findUntil(const int terminator, size_t length) const;
  uint8_t _parseInt(void) const;
  virtual const char _read(void) const;

  Stream* _currentStream;

  char _currentAction;
  uint8_t _currentRelay;

};



#endif
