#ifndef FLOW_H
#define FLOW_H

#define LED_BUILTIN 3
#define SENSOR  27

#include <Arduino.h>

void IRAM_ATTR pulseCounter();
void flowSetup();
void readFlow();

#endif 