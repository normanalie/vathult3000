#include "flows.h"

unsigned long first_t;
unsigned long second_t;
uint8_t flow_pulse_counter;

void IRAM_ATTR pulse_ISR(){
  flow_pulse_counter++;
}

Flows::Flows(uint8_t addr, uint8_t sda, uint8_t scl, uint8_t int_pin): PCF8574(addr, sda, scl, int_pin, pulse_ISR){
  this->int_pin = int_pin;
  for(int c=0; c<8; c++){
    this->pinMode(P0+c, INPUT);
  }
}

uint8_t Flows::read_pin(uint8_t pin, uint8_t timeout_ms){
  unsigned long t = millis();
  first_t = second_t = flow_pulse_counter = 0;
  this->attachInterrupt();
  while(flow_pulse_counter<=2 && millis()-t<timeout_ms){
    if(flow_pulse_counter==1){
      first_t = millis();
    }
    if(flow_pulse_counter==2){
      second_t = millis();
    }
  }
  this->detachInterrupt();
  return int( 1/((second_t - first_t)*7) );
}

void Flows::read_states(int16_t states[], uint8_t timeout_ms){
  uint8_t each_timeout = int( timeout_ms/2 );
  states[STATE_FLOW_TAP] = this->read_pin(P0, each_timeout);
  states[STATE_FLOW_RAIN] = this->read_pin(P1, each_timeout);
  return;
}