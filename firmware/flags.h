#ifndef FLAGS_H
#define FLAGS_H

#include "Arduino.h"

#define STATE_SOURCE    0
#define STATE_OUTPUT1   1
#define STATE_OUTPUT2   2 
#define STATE_OUTPUT3   3
#define STATE_OUTPUT4   4
#define STATE_PUMP      5
#define STATE_FLOW_TAP  6
#define STATE_FLOW_RAIN 7
#define STATE_WIFI      8 

extern int16_t states[16];

#endif