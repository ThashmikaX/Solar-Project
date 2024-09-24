#ifndef DS18B20_H
#define DS18B20_H

#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2
#define ONE_WIRE_BUS1 15

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
extern OneWire oneWire;
extern OneWire oneWire1;
extern DallasTemperature sensors;
extern DallasTemperature sensors1;

void setupDS();
void readDS();

#endif