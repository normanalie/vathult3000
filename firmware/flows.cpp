#include "esp32-hal.h"
#include "flows.h"

unsigned long first_t;
unsigned long second_t;
uint8_t flow_pulse_counter;

void IRAM_ATTR pulse_ISR(){
  flow_pulse_counter++;
}

Flows::Flows(uint8_t pin): {
  this->pin = pin;
  this->calibration_factor = 4.5;
}

uint8_t Flows::read_pin(uint8_t pin, uint8_t timeout_ms){
  // Reset pulse counter
  // Attach interrupt
  // Wait until pulsecounter reach threshold value or timeout
  // Detach interrupt
  // Calc flow rate
  flow_pulse_counter = 0;
  unsigned long start_t = millis();
  attachInterrupt(this->pin, pulse_ISR, FALLING);
  while(flow_pulse_counter < 255){
    if((millis() - start_t) < timeout_ms){
      return 0
    }
  } 
  unsigned long end_t = millis();
  detachInterrupt(this->pin);
  return ((1000.0 / (end_t - start_t)) * flow_pulse_counter) / this->calibration_factor;
}

/* == DEPRECATED due to new hardware architecture. ==
void Flows::read_states(int16_t states[], uint8_t timeout_ms){
  uint8_t each_timeout = int( timeout_ms/2 );
  states[STATE_FLOW_TAP] = this->read_pin(P0, each_timeout);
  states[STATE_FLOW_RAIN] = this->read_pin(P1, each_timeout);
  return;
}
*/