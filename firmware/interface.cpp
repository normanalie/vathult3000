#include "interface.h"

Screen::Screen(int sda, int scl) : U8G2_SSD1306_128X64_NONAME_1_HW_I2C(U8G2_R0, -1, scl, sda)
{
}

void Screen::update()
{
  this->firstPage();
  do
  {
    this->draw_headbar();
    this->draw_home();
  } while (this->nextPage());
  return;
}

void Screen::draw_headbar()
{
  this->setFont(u8g_font_7x14); // for sanity

  // WiFi
  uint8_t *buf = new uint8_t[2 * size_wifi];
  if (buf)
  {
    memcpy_P(buf, bmp_wifi, 2 * size_wifi);
    this->drawXBM(0, 0, size_wifi, size_wifi, buf);
  }

  // Warns
  this->drawStr(42, 11, "3");
  if (buf)
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
  this->setFont(u8g2_font_squeezed_b7_tr); // for sanity

  uint8_t *buf = new uint8_t[2 * size_valve];
  if (buf)
  {
    switch (this->selected_id)
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
  Serial.printf("{<LEFT: %d>, <CENTER: %d>, <RIGHT: %d>}\n", this->btn_status[0], this->btn_status[1], this->btn_status[2]);  // Test
  return;
}

void Keyboard::inter_handler()
{
  
  return;
}