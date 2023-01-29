#include "interface.h"

Screen::Screen(int sda, int scl) : U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, -1, scl, sda){

}

void Screen::update()
{
  this->firstPage();
  do
  {
    this->draw_headbar();
    switch(this->current_screen){
      case HOME:
        this->draw_home();
        break;
      case OUTPUTS:
        this->draw_outputs();
        break;
      case SENSORS:
        break;
      case SETTINGS:
        break;
    }
  } while (this->nextPage());
  return;
}

void Screen::nav_left(){
  uint8_t i = 0;
  while(i < BTN_BUF_LEN){
    if(i == BTN_BUF_LEN-1){
      for(int c=0; c<BTN_BUF_LEN; c++){
        this->press_buffer[c] = EMPTY;
      }
      this->press_buffer[0] = LEFT;
    }else if(this->press_buffer[i] == EMPTY){
      this->press_buffer[i] = LEFT;
      i = BTN_BUF_LEN;
    }
  }
  return;
}

void Screen::nav_center(){
  uint8_t i = 0;
  while(i < BTN_BUF_LEN){
    if(i == BTN_BUF_LEN-1){
      for(int c=0; c<BTN_BUF_LEN; c++){
        this->press_buffer[c] = EMPTY;
      }
      this->press_buffer[0] = CENTER;
    }else if(this->press_buffer[i] == EMPTY){
      this->press_buffer[i] = CENTER;
      i = BTN_BUF_LEN;
    }
  }
  return;
}

void Screen::nav_right(){
  uint8_t i = 0;
  while(i < BTN_BUF_LEN){
    if(i == BTN_BUF_LEN-1){
      for(int c=0; c<BTN_BUF_LEN; c++){
        this->press_buffer[c] = EMPTY;
      }
      this->press_buffer[0] = RIGHT;
    }else if(this->press_buffer[i] == EMPTY){
      this->press_buffer[i] = RIGHT;
      i = BTN_BUF_LEN;
    }
  }
  return;
}

void Screen::draw_headbar()
{
  this->setFont(u8g_font_7x14); // for sanity

  // WiFi
  uint8_t *buf = new uint8_t[2 * size_wifi];
  if(buf)
  {
    if(this->wifi_connected){
      memcpy_P(buf, bmp_wifi, 2 * size_wifi);
      this->drawXBM(0, 0, size_no_wifi, size_no_wifi, buf);
    }else{
      memcpy_P(buf, bmp_no_wifi, 2 * size_no_wifi);
      this->drawXBM(0, 0, size_no_wifi, size_no_wifi, buf);
    }
  }

  // Warns
  //this->drawStr(42, 11, "3");  // Warns count
  if(buf && this->error)
  {
    memcpy_P(buf, bmp_warn, 2 * size_warn);
    this->drawXBM(50, 0, size_warn, size_warn, buf);
  }

  // Time
  this->drawStr(94, 11, this->time);

  // Bottom line
  this->drawLine(0, 12, 128, 12);

  delete[] buf;
  return;
}

void Screen::draw_home()
{
  static uint8_t selected = 0;
  enum Buttons pressed = EMPTY;
  
  this->setFont(u8g2_font_squeezed_b7_tr); // for sanity

  pressed = this->pressed();
  if(pressed == LEFT){
    selected = selected==0 ? 2 : selected-1;
  }else if(pressed == CENTER){
    if(selected == 0) this->current_screen = OUTPUTS;
    if(selected == 1) this->current_screen = SENSORS;
    if(selected == 2) this->current_screen = SETTINGS;
    return;
  }else if(pressed == RIGHT){
    selected = selected==2 ? 0 : selected+1; 
  }

  uint8_t *buf = new uint8_t[2 * size_valve];
  if(buf)
  {
    switch (selected)
    {
    case 0:
      // Left item
      memcpy_P(buf, bmp_tool, 2 * size_tool);
      this->drawXBM(10, 31, size_tool, size_tool, buf);

      // Selected item
      memcpy_P(buf, bmp_valve, 2 * size_valve);
      this->drawXBM(56, 31, size_valve, size_valve, buf);
      this->drawStr(48, 55, "Outputs");
      this->drawFrame(42, 23, 47, 40);

      // Right item
      memcpy_P(buf, bmp_temperature, 2 * size_temperature);
      this->drawXBM(102, 31, size_temperature, size_temperature, buf);
      break;
    case 1:
      // Left item
      memcpy_P(buf, bmp_valve, 2 * size_valve);
      this->drawXBM(10, 31, size_valve, size_valve, buf);

      // Selected item
      memcpy_P(buf, bmp_temperature, 2 * size_temperature);
      this->drawXBM(56, 31, size_temperature, size_temperature, buf);
      this->drawStr(48, 55, "Sensors");
      this->drawFrame(42, 23, 47, 40);

      // Right item
      memcpy_P(buf, bmp_tool, 2 * size_tool);
      this->drawXBM(102, 31, size_tool, size_tool, buf);
      break;
    case 2:
      // Left item
      memcpy_P(buf, bmp_temperature, 2 * size_temperature);
      this->drawXBM(10, 31, size_temperature, size_temperature, buf);

      // Selected item
      memcpy_P(buf, bmp_tool, 2 * size_tool);
      this->drawXBM(56, 31, size_tool, size_tool, buf);
      this->drawStr(48, 55, "Settings");
      this->drawFrame(42, 23, 47, 40);

      // Right item
      memcpy_P(buf, bmp_valve, 2 * size_valve);
      this->drawXBM(102, 31, size_valve, size_valve, buf);
      break;
    }
  }

  delete[] buf;
  return;
}


void Screen::draw_outputs(){
  static int8_t source = -1;
  static uint8_t frame_y = 32;

  /* Logic */
  enum Buttons pressed = EMPTY;
  pressed = this->pressed();
  if(source == -1){
    if(pressed == LEFT) frame_y = frame_y==17 ? 17 : frame_y-15;
    if(pressed == RIGHT) frame_y = frame_y==47 ? 47 : frame_y+15;
    if(pressed == CENTER){
      if(frame_y==17){
        this->current_screen = HOME;
        frame_y = 32;
        return;
      }
      if(frame_y==32) source = 0;
      if(frame_y==47) source = 1;            
    }
  }

  /* Display */
  this->setFont(u8g2_font_squeezed_b7_tr); // for sanity
  uint8_t *buf = new uint8_t[2 * size_valve];
  if(buf)
  {
    if(source == -1){  // water source not set 
      /* Select water source */
      // Back
      memcpy(buf, bmp_back, 2*size_back);
      this->drawXBM(0, 20, size_back, size_back, buf);
      this->drawStr(13, 28, "Back");
      // Tap water
      this->drawStr(13, 43, "Tap water");
      // Rain water
      this->drawStr(13, 58, "Rain water");
      // Frame
      this->drawFrame(0, frame_y, 128, 16);
    }
  }
  delete[] buf;
  return;
}

enum Buttons Screen::pressed(){
  enum Buttons pressed = this->press_buffer[0];  // Store the oldest pressed button

  for(int c=1; c<BTN_BUF_LEN-1; c++){  // Slide the buffer
    this->press_buffer[c-1] = this->press_buffer[c];
  }
  this->press_buffer[BTN_BUF_LEN-1] = EMPTY;

  return pressed;
}


Keyboard::Keyboard(uint8_t addr, uint8_t sda, uint8_t scl, uint8_t inter_pin) : PCF8574(addr, sda, scl)
{
  this->pinMode(P0, INPUT_PULLUP);
  this->pinMode(P1, INPUT_PULLUP);
  this->pinMode(P2, INPUT_PULLUP);
}

void Keyboard::update()
{
  this->btn_status[0] = !this->digitalRead(P2);  // Right
  this->btn_status[1] = !this->digitalRead(P1);  // Center
  this->btn_status[2] = !this->digitalRead(P0);  // Left
  return;
}

bool Keyboard::isOnlyPressed(uint8_t btn){
  /* Return true if the button is currently the only one to be pressed. */ 
  switch(btn){
    case BTN_LEFT:
      if(this->btn_status[BTN_LEFT] && !this->btn_status[BTN_CENTER] && !this->btn_status[BTN_RIGHT]) return true;
      break;
    case BTN_CENTER:
      if(!this->btn_status[BTN_LEFT] && this->btn_status[BTN_CENTER] && !this->btn_status[BTN_RIGHT]) return true;
      break;
    case BTN_RIGHT:
      if(!this->btn_status[BTN_LEFT] && !this->btn_status[BTN_CENTER] && this->btn_status[BTN_RIGHT]) return true;
      break;
  }
  return false;
}

void Keyboard::inter_handler()
{
  
  return;
}


