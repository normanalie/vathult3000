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
      screen.selected_id = screen.selected_id == 0 ? MENU_ITEM_LEN-1 : screen.selected_id-1;
    }else if(keyboard.isOnlyPressed(BTN_RIGHT)){
      screen.selected_id = screen.selected_id == MENU_ITEM_LEN-1 ? 0 : screen.selected_id+1;
    }
    t = millis();
  }
  return;
}