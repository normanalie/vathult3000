#ifndef FLAGS_H
#define FLAGS_H

#include "Arduino.h"

#define SOURCE_TAP  0
#define SOURCE_RAIN 1

#define STATE_SOURCE        0
#define STATE_OUTPUT1       1
#define STATE_OUTPUT2       2 
#define STATE_OUTPUT3       3
#define STATE_OUTPUT4       4
#define STATE_PUMP          5
#define STATE_FLOW          6
#define STATE_WIFI          7 
#define STATE_INFILTRATION  8

extern int16_t states[16];

#endif