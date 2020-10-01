/*
 * Wright Master Relay
 * 
 * https://github.com/1e1/arduino-webcontroller-relay
 *
 */



#include <Arduino.h>



/** ===================== **/




void setup()
{
    Serial.begin(9600);
    Serial.println("setup");
}


void loop()
{
  Serial.println("loop");
  delay(60000);
}
