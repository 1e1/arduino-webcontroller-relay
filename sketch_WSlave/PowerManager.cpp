#include "PowerManager.h"




/***********************************************************
 *                       PROPERTIES                        *
 **********************************************************/








/***********************************************************
 *                         PUBLIC                          *
 **********************************************************/




PowerManager::PowerManager(const Frequency frequency)
{
  #if WS_LOWPOWER & WS_LOWPOWER_DOWNCLOCK
  LOGLN(F("lowpower: downclock"));
  clock_prescale_set((clock_div_t) frequency);
  #endif
}




void PowerManager::begin()
{
  #if (WS_LOWPOWER & WS_LOWPOWER_NO_ADC) && (NUM_ANALOG_INPUTS > 0)
  LOGLN(F("lowpower: no ADC"));
  power_adc_disable();
  #endif

  #if (WS_LOWPOWER & WS_LOWPOWER_NO_SPI) && !(WS_INTERFACE & WS_INTERFACE_ETHERNET)
  LOGLN(F("lowpower: no SPI"));
  power_spi_disable();
  #endif

  #if WS_LOWPOWER & WS_LOWPOWER_NO_TWI
  LOGLN(F("lowpower: no I2C"));
  power_twi_disable();
  #endif

  #if WS_LOWPOWER & WS_LOWPOWER_NO_USART && !(WS_INTERFACE & WS_INTERFACE_USB) && (WS_LOG_LEVEL == WS_LOG_LEVEL_OFF)
  LOGLN(F("lowpower: no usart"));
  power_usart0_disable();
  #endif

  #if WS_LOWPOWER & WS_LOWPOWER_DOWNCLOCK
  LOGLN(F("lowpower: downclock1&2"));
  power_timer1_disable();
  power_timer2_disable();
  #endif
}


const uint8_t PowerManager::update()
{
  #if WS_LOWPOWER & WS_LOWPOWER_HAS_DELAY
  delay(WS_LOWPOWER_DELAY_MS);
  #endif
}
