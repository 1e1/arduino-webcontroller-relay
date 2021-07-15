#include "PowerManager.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/



static const bool _USE_ADC = !(WS_LOWPOWER & WS_LOWPOWER_NO_ADC);
static const bool _USE_SPI = !(WS_LOWPOWER & WS_LOWPOWER_NO_SPI) || (WS_INTERFACE & WS_INTERFACE_ETHERNET) || (WS_INTERFACE & WS_INTERFACE_WIFI);
static const bool _USE_MILLIS = !(WS_LOWPOWER & WS_LOWPOWER_DOWNCLOCK) || (WS_INTERFACE & WS_INTERFACE_ETHERNET) || (WS_INTERFACE & WS_INTERFACE_WIFI) || (WS_INTERFACE & WS_INTERFACE_USB) || (WS_INTERFACE & WS_INTERFACE_SERIAL) || (WS_LOG_LEVEL != WS_LOG_LEVEL_OFF);
static const bool _USE_TWI = !(WS_LOWPOWER & WS_LOWPOWER_NO_TWI);
static const bool _USE_USART = !(WS_LOWPOWER & WS_LOWPOWER_NO_USART) || (WS_INTERFACE & WS_INTERFACE_USB) || (WS_INTERFACE & WS_INTERFACE_SERIAL) || (WS_LOG_LEVEL != WS_LOG_LEVEL_OFF);


void _ISR_wakeup()
{
}


#if (WS_LOWPOWER & WS_LOWPOWER_HAS_DELAY)
EMPTY_INTERRUPT(WDT_vect);
#endif

#if (WS_LOWPOWER & WS_LOWPOWER_INTERRUPTS)
  #if WS_INTERFACE & WS_INTERFACE_LCD
  ISR(PCINT2_vect)
  {
    //_ISR_wakeup();
    // check analog input
  }
  #endif
  #if WS_INTERFACE & WS_INTERFACE_SERIAL
  ISR(PCINT1_vect)
  {
    _ISR_wakeup();
  }
  #endif
  #if WS_INTERFACE & WS_INTERFACE_ETHERNET
  ISR(PCINT0_vect)
  {
    //_ISR_wakeup();
    // check Ethernet
  }
  #endif


  void attachDefaultInterrupts(const uint8_t mode = RISING)
  {
    //noInterrupts();
    /*
    PCIE2 = PCINT23:16
    PCIE1 = PCINT15:8
    PCIE0 = PCINT7:0
    */
    #if WS_INTERFACE & WS_INTERFACE_LCD
      bitSet(PCICR, PCIE2);
      #ifdef PCINT16  // A8
        bitSet(PCMSK2, PCINT16);
      #endif
    #endif
    #if WS_INTERFACE & WS_INTERFACE_SERIAL
      bitSet(PCICR, PCIE1);
      #if WS_SERIAL_ID == 3
        #ifdef PCINT9 // RX3
          bitSet(PCMSK1, PCINT9);
        #endif
      #endif
      #if (WS_SERIAL_ID == 0)) || (WS_INTERFACE & WS_INTERFACE_USB)
        #ifdef PCINT8 // RX0
          bitSet(PCMSK1, PCINT8);
        #endif
      #endif
      #if WS_SERIAL_ID == 1
        #ifdef PCINT9 // RX1
          bitSet(PCMSK1, PCINT8);
        #endif
      #endif
    #endif
    #if WS_INTERFACE & WS_INTERFACE_ETHERNET
      bitSet(PCICR, PCIE0);
      #ifdef PCINT3  // MISO
        // bitSet(PCMSK0, PCINT3);
      #endif
    #endif
    //interrupts();
  }

  void detachDefaultInterrupts()
  {
    //noInterrupts();
    /*
    PCIE2 = PCINT23:16
    PCIE1 = PCINT15:8
    PCIE0 = PCINT7:0
    */
    #if WS_INTERFACE & WS_INTERFACE_LCD
      bitSet(PCICR, PCIE2);
      #ifdef PCINT16  // A8
        bitClear(PCMSK2, PCINT16);
      #endif
    #endif
    #if WS_INTERFACE & WS_INTERFACE_SERIAL
      bitSet(PCICR, PCIE1);
      #if WS_SERIAL_ID == 3
        #ifdef PCINT9 // RX3
          bitClear(PCMSK1, PCINT9);
        #endif
      #endif
      #if (WS_SERIAL_ID == 0)) || (WS_INTERFACE & WS_INTERFACE_USB)
        #ifdef PCINT8 // RX0
          bitClear(PCMSK1, PCINT8);
        #endif
      #endif
      #if WS_SERIAL_ID == 1
        #ifdef PCINT9 // RX1
          bitClear(PCMSK1, PCINT8);
        #endif
      #endif
    #endif
    #if WS_INTERFACE & WS_INTERFACE_ETHERNET
      bitSet(PCICR, PCIE0);
      #ifdef PCINT3  // MISO
        // bitClear(PCMSK0, PCINT3);
      #endif
    #endif
    //interrupts();
  }
#endif

#else
  void attachDefaultInterrupts(const uint8_t mode = RISING) {};
  void detachDefaultInterrupts() {};
#endif

uint8_t PowerManager::_cycleCounter = 0;




/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




void PowerManager::begin()
{
  for (uint8_t pin; pin<NUM_DIGITAL_PINS; ++pin) {
    pinMode(pin, INPUT_PULLUP);
  }
  
  PowerManager::_disableComponents();
  PowerManager::_enableComponents();
}


void PowerManager::loop()
{
  /*
  SLEEP_MODE_IDLE: 15 mA
  SLEEP_MODE_ADC: 6.5 mA
  SLEEP_MODE_PWR_SAVE: 1.62 mA
  SLEEP_MODE_EXT_STANDBY: 1.62 mA
  SLEEP_MODE_STANDBY : 0.84 mA
  SLEEP_MODE_PWR_DOWN : 0.36 mA
  */
  if (PowerManager::_cycleCounter < WS_IDLE_LOOP) {
    #if (WS_LOWPOWER & WS_LOWPOWER_AUTOSLEEP)
      PowerManager::_cycleCounter++;
    #endif

    #if (WS_LOWPOWER & WS_LOWPOWER_HAS_DELAY)
      PowerManager::_sleepTimer(SLEEP_MODE_IDLE, Wdt::WS_IDLE_TIME);
    #endif
  } else {
    LOGLN(F("sleeping..."));

    #if WS_LOWPOWER & WS_LOWPOWER_INTERRUPTS
      LOGLN(F("unlimited"));
      PowerManager::_sleepUntilInterrupt(SLEEP_MODE_PWR_DOWN);
    #if (WS_LOWPOWER & WS_LOWPOWER_AUTOSLEEP)
      PowerManager::hold();
    #endif
    #else
      LOGLN(F("time"));
      PowerManager::_sleepTimer(SLEEP_MODE_PWR_DOWN, Wdt::WS_SLEEP_TIME);
    #endif

    delay(WS_LOWPOWER_WAKEUP_BOD_MS);
    LOGLN(F("awaken!"));
  }
}


const bool PowerManager::isFirstLoop()
{
  return PowerManager::_cycleCounter == 0;
}


void PowerManager::highCpu()
{
  #if (WS_LOWPOWER & WS_LOWPOWER_DOWNCLOCK)
  LOGLN(F("lowpower: restore clock"));
  clock_prescale_set((clock_div_t) Frequency::F_16MHz);
  #endif
}


void PowerManager::lowCpu()
{
  #if (WS_LOWPOWER & WS_LOWPOWER_DOWNCLOCK)
  LOGLN(F("lowpower: downclock"));
  clock_prescale_set((clock_div_t) Frequency::F_2MHz);
  #endif
}


void PowerManager::sleep()
{
  PowerManager::_cycleCounter = WS_IDLE_LOOP;
}


void PowerManager::hold()
{
  PowerManager::_cycleCounter = 0;
}



/***********************************************************
 *                        PROTECTED                        *
 **********************************************************/




void PowerManager::_disableComponents()
{
  power_all_disable();
}  


void PowerManager::_enableComponents()
{
  if (_USE_ADC) {
    LOGLN(F("lowpower: enable ADC"));
    power_adc_enable();
  } else {
    LOGLN(F("lowpower: disable ADC"));
    //power_adc_disable();
  }

  if (_USE_SPI) {
    LOGLN(F("lowpower: enable SPI"));
    power_spi_enable();
  } else {
    LOGLN(F("lowpower: disable SPI"));
    //power_spi_disable();
  }

  if (_USE_MILLIS) {
    LOGLN(F("lowpower: enable downclock1&2"));
    power_timer0_enable(); // delay(), millis(), micro()
  } else {
    LOGLN(F("lowpower: disable downclock1&2"));
    //power_timer1_disable(); // Servo, pwm
    //power_timer2_disable(); // tone(), pwm
  }

  if (_USE_TWI) {
    LOGLN(F("lowpower: enable I2C"));
    power_twi_enable();
  } else {
    LOGLN(F("lowpower: disable I2C"));
    //power_twi_disable();
  }

  if (_USE_USART) {
    LOGLN(F("lowpower: enable usart"));
    #if (WS_INTERFACE & WS_INTERFACE_USB) || (WS_LOG_LEVEL != WS_LOG_LEVEL_OFF)
    power_usart0_enable();
    #endif
    #if defined(power_usart0_enable) && (WS_SERIAL_ID == 0)
    power_usart0_enable();
    #elif defined(power_usart1_enable) && (WS_SERIAL_ID == 1)
    power_usart1_enable();
    #elif defined(power_usart2_enable) && (WS_SERIAL_ID == 2)
    power_usart2_enable();
    #elif defined(power_usart3_enable) && (WS_SERIAL_ID == 3)
    power_usart3_enable();
    #endif
  } else {
    LOGLN(F("lowpower: disable usart"));
    //power_usart0_disable();
  }
}


void PowerManager::_sleepTimer(const byte mode, const Wdt timer)
{
  noInterrupts();         // timed sequence follows 
  ADCSRA &= ~(1 << ADEN); // disable ADC
  MCUSR = 0;              // clear various "reset" flags
  
  if (_USE_MILLIS) {
    power_timer0_disable();
  }
  WDTCSR |= bit(WDCE) | bit(WDE);  // allow changes, disable reset
  WDTCSR = timer;                 // set interrupt mode and an interval
  //wdt_reset();                    // pat the dog
  
  sleep_enable();
  set_sleep_mode(mode);
  interrupts();     // guarantees next instruction executed
  sleep_cpu();
  sleep_disable();  // cancel sleep as a precaution
  
  if (_USE_MILLIS) {
    power_timer0_enable();
  }
  wdt_disable();
  ADCSRA |= (1 << ADEN);
}


void PowerManager::_sleepUntilInterrupt(const byte mode)
{
  noInterrupts();         // timed sequence follows 
  ADCSRA &= ~(1 << ADEN); // disable ADC
  MCUSR = 0;              // clear various "reset" flags
  attachDefaultInterrupts();
  
  sleep_enable();
  set_sleep_mode(mode);
  interrupts();     // guarantees next instruction executed
  sleep_cpu();
  sleep_disable();  // cancel sleep as a precaution
  
  detachDefaultInterrupts();
  ADCSRA |= (1 << ADEN);
}
