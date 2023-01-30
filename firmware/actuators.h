#ifndef ACTUATORS_H
#define ACTUATORS_H

#include "Arduino.h"
#include <PCF8574.h>

#include "flags.h"

class Actuators: public PCF8574{
  public:
    Actuators(uint8_t addr, uint8_t sda, uint8_t scl);
    void write_states(int16_t state[]);
  private:
};

#endif