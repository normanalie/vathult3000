#ifndef INTERFACE_H
#define INTERFACE_H

#include "Arduino.h"
#include <PCF8574.h>
#include <U8g2lib.h>  // OLED Screen

#include "bitmaps.h"

class Screen: public U8G2_SSD1306_128X64_NONAME_1_HW_I2C{
  public:
    Screen(int sda, int scl);
    void update();
    bool wifi_connected = false;
    bool error = false;
  private:
    char time[6] = "12:12";  // TODO: Take a time object in constructor and uodate time in screen::update
};

#endif