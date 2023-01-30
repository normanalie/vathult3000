#ifndef FLOWS_H
#define FLOWS_H

#include "Arduino.h"
#include <PCF8574.h>
#include "flags.h"

void IRAM_ATTR pulse_ISR();

class Flows: public PCF8574{
  public:
    Flows(uint8_t addr, uint8_t sda, uint8_t scl, uint8_t int_pin);
    void read_states(int16_t state[], uint8_t timeout_ms);
  private:
    uint8_t read_pin(uint8_t pin, uint8_t timeout_ms);
    uint8_t int_pin;
};

#endif