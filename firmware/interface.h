#ifndef INTERFACE_H
#define INTERFACE_H

#define MENU_ITEM_LEN 3 
#define BTN_LEFT 0
#define BTN_CENTER 1
#define BTN_RIGHT 2

#include "Arduino.h"
#include <PCF8574.h>
#include <U8g2lib.h>  // OLED Screen
#include <string.h>

#include "bitmaps.h"

class Screen: public U8G2_SSD1306_128X64_NONAME_1_HW_I2C{
  public:
    Screen(int sda, int scl);
    void update();
    void draw_outputs();
    bool wifi_connected = false;
    bool error = false;
    uint8_t selected_id = 0;
  private:
    char time[6] = "12:12";  // TODO: Take a time object in constructor and update time in screen::update
    void draw_headbar();
    void draw_home();
};

class Keyboard: public PCF8574{
  public:
    Keyboard(uint8_t addr, uint8_t sda, uint8_t scl, uint8_t inter_pin);
    void update();
    bool isOnlyPressed(uint8_t btn);  // Return true if the button is currently the only one to be pressed.
  private:
    static void inter_handler();
    bool btn_status[3] = {0};
};


#endif