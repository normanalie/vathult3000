/*
============== VÂTHULT3000 ==============
--- Smart multi-zones watering system ---

Firmware V0.1
 - Developped by: Norman ALIE, Julian DOLOIR, Victor MEULIN
 - Society: ISTY
 - Contact: Norman ALIE <mail@normanalie.fr>
 - License: CC BY-SA

For informations about wiring see the "hardware" folder.
For informations about controll see "software" folder.
*/

/* SELECT BOARD */
//#define ESP32  // Production version
#define ESP8266  // Prototype version

#include "interface.h"

#ifdef ARDUINO
  #define SDA A4
  #define SCL A5
#endif
#ifdef ESP8266
  #define SDA D4
  #define SCL D5
#endif
#ifdef ESP32
  #define SDA 0
  #define SCL 1
#endif

Screen screen = Screen(SDA, SCL);

void setup(){
  Serial.begin(9600);

  screen.begin();
}

void loop(){
  screen.update();
  delay(1000);
}
