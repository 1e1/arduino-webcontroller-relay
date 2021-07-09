#ifndef MorseLed_h
#define MorseLed_H_



#include <Arduino.h>
#include "config.h"
#include "macro.h"


static constexpr const unsigned short shortDuration = 250; // TODO CONSTANTIZE
static constexpr const unsigned short longDuration = shortDuration * 3;
static constexpr const unsigned short endSymbolDuration = shortDuration;
static constexpr const unsigned short endCharDuration = shortDuration * 3;
static constexpr const unsigned short endWordDuration = shortDuration * 7;

static constexpr const unsigned short endSymbolDelay = endSymbolDuration;
static constexpr const unsigned short endCharDelay = endCharDuration - endSymbolDelay;
static constexpr const unsigned short endWordDelay = endWordDuration - endCharDelay;


class MorseLed {

  public:
  MorseLed(const uint8_t pin) : _pin(pin) { pinMode(pin, OUTPUT); digitalWrite(this->_pin, WM_LED_OFF); };

  void d0(void) { this->_T();this->_T();this->_T();this->_T();this->_T(); this->_charEnd(); };
  void d1(void) { this->_x();this->_T();this->_T();this->_T();this->_T(); this->_charEnd(); };
  void d2(void) { this->_x();this->_x();this->_T();this->_T();this->_T(); this->_charEnd(); };
  void d3(void) { this->_x();this->_x();this->_x();this->_T();this->_T(); this->_charEnd(); };
  void d4(void) { this->_x();this->_x();this->_x();this->_x();this->_T(); this->_charEnd(); };
  void d5(void) { this->_x();this->_x();this->_x();this->_x();this->_x(); this->_charEnd(); };
  void d6(void) { this->_T();this->_x();this->_x();this->_x();this->_x(); this->_charEnd(); };
  void d7(void) { this->_T();this->_T();this->_x();this->_x();this->_x(); this->_charEnd(); };
  void d8(void) { this->_T();this->_T();this->_T();this->_x();this->_x(); this->_charEnd(); };
  void d9(void) { this->_T();this->_T();this->_T();this->_T();this->_x(); this->_charEnd(); };

  void a(void) { this->_x();this->_T(); this->_charEnd(); };
  void b(void) { this->_T();this->_x();this->_x();this->_x(); this->_charEnd(); };
  void c(void) { this->_T();this->_x();this->_T();this->_x(); this->_charEnd(); };
  void d(void) { this->_T();this->_x();this->_x(); this->_charEnd(); };
  void e(void) { this->_x(); this->_charEnd(); };
  void f(void) { this->_x();this->_x();this->_T();this->_x(); this->_charEnd(); };
  void g(void) { this->_T();this->_T();this->_x(); this->_charEnd(); };
  void h(void) { this->_x();this->_x();this->_x();this->_x(); this->_charEnd(); };
  void i(void) { this->_x();this->_x(); this->_charEnd(); };
  void j(void) { this->_x();this->_T();this->_T();this->_T(); this->_charEnd(); };
  void k(void) { this->_T();this->_x();this->_T(); this->_charEnd(); };
  void l(void) { this->_x();this->_T();this->_x();this->_x(); this->_charEnd(); };
  void m(void) { this->_T();this->_T(); this->_charEnd(); };
  void n(void) { this->_T();this->_x(); this->_charEnd(); };
  void o(void) { this->_T();this->_T();this->_T(); this->_charEnd(); };
  void p(void) { this->_x();this->_T();this->_T();this->_x(); this->_charEnd(); };
  void q(void) { this->_T();this->_T();this->_x();this->_T(); this->_charEnd(); };
  void r(void) { this->_x();this->_T();this->_x(); this->_charEnd(); };
  void s(void) { this->_x();this->_x();this->_x(); this->_charEnd(); };
  void t(void) { this->_T(); this->_charEnd(); };
  void u(void) { this->_x();this->_x();this->_T(); this->_charEnd(); };
  void v(void) { this->_x();this->_x();this->_x();this->_T(); this->_charEnd(); };
  void w(void) { this->_x();this->_T();this->_T(); this->_charEnd(); };
  void x(void) { this->_T();this->_x();this->_x();this->_T(); this->_charEnd(); };
  void y(void) { this->_T();this->_x();this->_T();this->_T(); this->_charEnd(); };
  void z(void) { this->_T();this->_T();this->_x();this->_x(); this->_charEnd(); };

  void space(void) const { this->_wordEnd(); };

  void print(const char message[])
  {
    uint8_t i = 0;
    do {
      switch(message[i]) {
        case '0': this->d0(); break;
        case '1': this->d1(); break;
        case '2': this->d2(); break;
        case '3': this->d3(); break;
        case '4': this->d4(); break;
        case '5': this->d5(); break;
        case '6': this->d6(); break;
        case '7': this->d7(); break;
        case '8': this->d8(); break;
        case '9': this->d9(); break;

        case 'a': this->a(); break;
        case 'b': this->b(); break;
        case 'c': this->c(); break;
        case 'd': this->d(); break;
        case 'e': this->e(); break;
        case 'f': this->f(); break;
        case 'g': this->g(); break;
        case 'h': this->h(); break;
        case 'i': this->i(); break;
        case 'j': this->j(); break;
        case 'k': this->k(); break;
        case 'l': this->l(); break;
        case 'm': this->m(); break;
        case 'n': this->n(); break;
        case 'o': this->o(); break;
        case 'p': this->p(); break;
        case 'q': this->q(); break;
        case 'r': this->r(); break;
        case 's': this->s(); break;
        case 't': this->t(); break;
        case 'u': this->u(); break;
        case 'v': this->v(); break;
        case 'w': this->w(); break;
        case 'x': this->x(); break;
        case 'y': this->y(); break;
        case 'z': this->z(); break;

        case ' ': this->space(); break;

        default: return;
      }
    } while (i++ != ((uint8_t) -1));
  };

  protected:
  void _display(const short duration) const
  { 
    digitalWrite(this->_pin, WM_LED_ON);
    delay(duration);
    digitalWrite(this->_pin, WM_LED_OFF);
    delay(endSymbolDelay);
  };

  void _T(void) const { this->_display(longDuration); };
  void _x(void) const { this->_display(shortDuration); };
  void _charEnd(void) const { delay(endCharDelay); };
  void _wordEnd(void) const { delay(endWordDelay); };

  const uint8_t _pin;
};



#endif
