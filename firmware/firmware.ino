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

bool outputs_state[4] = {0};
uint8_t input_source = -1;

Screen screen = Screen(SDA, SCL);
Keyboard keyboard = Keyboard(0b0100010, SDA, SCL, -1);

void menu_navigate();

void setup(){
  Serial.begin(9600);
  screen.begin();
  keyboard.begin();
}

void loop(){
  screen.update();
  keyboard.update();
  menu_navigate();
}

void menu_navigate(){
  static unsigned long t = millis();
  if(millis()-t > 150){
    if(keyboard.isOnlyPressed(BTN_LEFT)){
      screen.nav_left();
    }else if(keyboard.isOnlyPressed(BTN_RIGHT)){
      screen.nav_right();
    }else if(keyboard.isOnlyPressed(BTN_CENTER)){
      screen.nav_center();
    }
    t = millis();
  }
  return;
}
