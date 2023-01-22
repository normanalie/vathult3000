#include "interface.h"

Screen::Screen(int sda, int scl): U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, -1, scl, sda){

}

void Screen::update(){
  this->firstPage();
  do{
    // WiFi
    uint8_t *buf = new uint8_t[22];
    if(buf){
      //memcpy_P(buf, bitmap_icon_wifi, 22);
      this->drawXBM(0, 0, 11, 11, bmp_wifi);

      this->drawXBM(15, 0, 11, 11, bmp_no_wifi);

      this->drawXBM(30, 0, 10, 11, bmp_warn);
    }


  }while(this->nextPage());
  return;
}