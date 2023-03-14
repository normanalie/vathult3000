#include "actuators.h"

Actuators::Actuators(uint8_t addr, uint8_t sda, uint8_t scl): PCF8574(addr, sda, scl){
  for(int c=0; c<8; c++){
    this->pinMode(P0+c, OUTPUT, LOW);
  }
}

void Actuators::write_states(int16_t states[]){
  // Turn off pump 
  if(!states[STATE_PUMP]){
    this->digitalWrite(P6, LOW);
    this->digitalWrite(P7, LOW);
  }
  // Select the correct input 
  if(states[STATE_SOURCE] == SOURCE_TAP){
    this->digitalWrite(P1, LOW);
    this->digitalWrite(P0, HIGH);
  }else if(states[STATE_SOURCE] == SOURCE_RAIN){
    this->digitalWrite(P0, LOW);
    this->digitalWrite(P1, HIGH);
  }
  // Set the outputs
  for(int c=0; c<4; c++){
    this->digitalWrite(P2+c, states[STATE_OUTPUT1+c]);
  }
  // Turn on pump 
  if(states[STATE_PUMP]){
    this->digitalWrite(P6, HIGH);
    this->digitalWrite(P7, HIGH);
  }
  return;
}