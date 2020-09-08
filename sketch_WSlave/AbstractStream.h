#ifndef AbstractStream_h
#define AbstractStream_h



#include <Arduino.h>
#include <avr/pgmspace.h>
#include "config.h"
#include "macro.h"
#include "AbstractInterface.h"
#include "Relay.h"



#define ACTION_ALL          '$'
#define ACTION_READ         'r'
#define ACTION_WRITE        'w'
#define ACTION_MAP          'm'
#define ACTION_NC           'c'
#define ACTION_NO           'o'
#define ACTION_SAVE         '!'
#define ACTION_RESET        '~'
#define PATH_SEPARATOR      '/'
#define LF                  '\n'
#define SEP                 ' '
#define TEXT_NC             F(" NC ")
#define TEXT_NO             F(" NO ")
#define RELAY_NONE          -1
#define BUFFER_SIZE         8




class AbstractStream : public AbstractInterface {

  public:
  bool read();
  void process();
  void terminate();

  // inline
  __attribute__((always_inline)) inline void setStream(Stream* inputStream) { this->_currentStream = inputStream; };

  protected:
  void _showOne(const uint8_t relay);
  void _showAll();

  bool _findUntil(const int terminator, size_t length);
  uint8_t _parseInt();
  virtual char _read();

  Stream* _currentStream;

  char _currentAction;
  uint8_t _currentRelay;

};



#endif
