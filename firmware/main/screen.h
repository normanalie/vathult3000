#ifndef SCREEN_H
#define SCREEN_H

#include "Arduino.h"
#include <Adafruit_SSD1306.h>

class Screen: public Adafruit_SSD1306{
  public:
    Screen(int sda, int scl);
    void print(String str);
    int fps;
  private:
    int sda, scl;
};

#endif