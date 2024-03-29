#ifndef INTERFACE_H
#define INTERFACE_H

#define BTN_BUF_LEN 3
#define ERR_BUF_LEN 8
#define BTN_LEFT 0
#define BTN_CENTER 1
#define BTN_RIGHT 2

#include "Arduino.h"
#include <PCF8574.h>
#include <U8g2lib.h>  // OLED Screen
#include <string.h>

#include "flags.h"
#include "bitmaps.h"

enum Screens{
  HOME,
  OUTPUTS,
  SENSORS,
  SETTINGS
};

enum Buttons{
  EMPTY,
  LEFT,
  CENTER,
  RIGHT
};

class Screen: public U8G2_SSD1306_128X64_NONAME_1_HW_I2C{
  public:
    Screen(int sda, int scl, int16_t *flags);
    void update();
    void nav_left();
    void nav_center();
    void nav_right();
    String error = "";
  private:
    char time[6] = "12:12";  // TODO: Take a time object in constructor and update time in screen::update       
    enum Screens current_screen = HOME;
    enum Buttons press_buffer[BTN_BUF_LEN] = {EMPTY};
    int16_t *flags;
    void draw_headbar();
    void draw_home();
    void draw_outputs();
    void draw_settings();
    void draw_sensors();
    enum Buttons pressed();
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