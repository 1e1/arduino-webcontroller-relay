#ifndef AbstractStream_h
#define AbstractStream_h



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
  bool read(void);
  void process(void);
  void terminate(void);

  // inline
  __attribute__((always_inline)) inline void setStream(Stream* inputStream) { this->_currentStream = inputStream; };

  protected:
  void _printData(const uint8_t data);
  void _printOne(const uint8_t relay);
  void _printAbout(void);
  void _printAll(void);

  bool _findUntil(const int terminator, size_t length);
  uint8_t _parseInt(void);
  virtual char _read(void);

  Stream* _currentStream;

  char _currentAction;
  uint8_t _currentRelay;

};



#endif
