#ifndef INTERFACE_H
#define INTERFACE_H

#define MENU_ITEM_LEN 3 

#include "Arduino.h"
#include <PCF8574.h>
#include <U8g2lib.h>  // OLED Screen
#include <string.h>

#include "bitmaps.h"

class Screen: public U8G2_SSD1306_128X64_NONAME_1_HW_I2C{
  public:
    Screen(int sda, int scl);
    void update();
    bool wifi_connected = false;
    bool error = false;
  private:
    char time[6] = "12:12";  // TODO: Take a time object in constructor and update time in screen::update
    void draw_headbar();
    void draw_home();
    uint8_t selected_id = 0;
};

class Keyboard: public PCF8574{
  public:
    Keyboard(uint8_t addr, uint8_t sda, uint8_t scl, uint8_t inter_pin);
    void update();
    bool btn_status[3] = {0};
  private:
    static void inter_handler();
};

#endif