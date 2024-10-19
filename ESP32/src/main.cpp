#include <Arduino.h>
#include <dht_11.h>
#include <flow.h>
#include <ds18b20.h>
#include <Wire.h>

void setup()
{
  Serial.begin(115200);
  flowSetup();
  // setupDHT();
  // setupDS();
}

void loop()
{
  readFlow();
  // readDHT();
  // readDS();
}