#include "interface.h"

Screen::Screen(int sda, int scl): U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, -1, scl, sda){

}

void Screen::update(){
  this->firstPage();
  do{
    this->draw_headbar();
    this->draw_home();
  }while(this->nextPage());
  return;
}

void Screen::draw_headbar(){
  this->setFont(u8g_font_7x14); // for sanity
  // WiFi
  uint8_t *buf = new uint8_t[2*size_wifi];
  if(buf){
    memcpy_P(buf, bmp_wifi, 2*size_wifi);
    this->drawXBM(0, 0, size_wifi, size_wifi, buf);
  }

  // Warns
  this->drawStr(42, 11, "3");
  if(buf){
    memcpy_P(buf, bmp_warn, 2*size_warn);
    this->drawXBM(50, 0, size_warn, size_warn, buf);
  }

  // Time
  this->drawStr(94, 11, this->time);

  // Bottom line
  this->drawLine(0, 12, 128, 12);

  delete [] buf;
  return;
}

void Screen::draw_home(){
  this->setFont(u8g2_font_squeezed_b7_tr); // for sanity

  uint8_t *buf = new uint8_t[2*size_valve];
  if(buf){
    // Left item
    memcpy_P(buf, bmp_valve, 2*size_valve);
    this->drawXBM(10, 31, size_valve, size_valve, buf);

    // Selected item
    memcpy_P(buf, bmp_temperature, 2*size_temperature);
    this->drawXBM(56, 31, size_temperature, size_temperature, buf);
    this->drawStr(48, 55, "Sensors");
    this->drawFrame(42, 23, 47, 40);

    // Right item
    memcpy_P(buf, bmp_tool, 2*size_tool);
    this->drawXBM(102, 31, size_tool, size_tool, buf);   
  }

  delete [] buf;
  return;
}