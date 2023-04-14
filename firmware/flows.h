#include <sys/_stdint.h>
#ifndef FLOWS_H
#define FLOWS_H

#include "Arduino.h"
//#include <PCF8574.h>
#include "flags.h"

#define CAIBRATION_FACTOR 4.5

void IRAM_ATTR pulse_ISR();

class Flows{
  public:
    Flows(uint8_t pin);
    uint8_t read_pin(uint8_t pin, uint8_t timeout_ms);
    /* void read_states(int16_t state[], uint8_t timeout_ms);  == DEPRECATED due to new hardware architecture. == */
  private:
    uint8_t pin;
    uint8_t calibration_factor;
};

#endif