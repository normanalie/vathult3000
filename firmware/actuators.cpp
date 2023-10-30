#include "actuators.h"

Actuators::Actuators(uint8_t addr, uint8_t sda, uint8_t scl): PCF8574(addr, sda, scl){
  for(int c=0; c<8; c++){
    this->pinMode(P0+c, OUTPUT, LOW);
  }
}

void Actuators::write_states(int16_t states[]){
  //INVERSE LOGIC ---- HIGH = OFF ------
  // Turn off pump 
  if(!states[STATE_PUMP]){
    this->digitalWrite(P6, HIGH);
  }
  // Select the correct input 
  if(states[STATE_SOURCE] == SOURCE_TAP){
    this->digitalWrite(P1, HIGH);
    this->digitalWrite(P0, LOW);
  }else if(states[STATE_SOURCE] == SOURCE_RAIN){
    this->digitalWrite(P0, HIGH);
    this->digitalWrite(P1, LOW);
  }
  // Set the outputs
  for(int c=0; c<4; c++){
    this->digitalWrite(P2+c, !states[STATE_OUTPUT1+c]);
  }
  //TUrn off inputs if all outputs to low
  if(!states[STATE_OUTPUT1] && !states[STATE_OUTPUT2] && !states[STATE_OUTPUT3] && !states[STATE_OUTPUT4]){
    this->digitalWrite(P0, HIGH);
    this->digitalWrite(P1, HIGH);
  }
  // Turn on pump 
  if(states[STATE_PUMP]){
    this->digitalWrite(P6, LOW);
  }
  return;
}