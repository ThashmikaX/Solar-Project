#include <Arduino.h>
#include <dht_11.h>
#include <flow.h>
#include <ds18b20.h>
#include <Wire.h>
#include "globals.h"

void setup()
{
    Serial.begin(115200);
    setup_wifi();
    client.setServer("mqtt_server", 1883);
    client.setCallback(callback);
    setupDHT();
    // setupDS();
    // flowSetup();
}

void loop() {
  readDHT();
  // readDS();
  // readFlow();
  
  if (!client.connected()) {
    reconnect();
    }
    client.loop();

    long now = millis();
    if (now - lastMsg > 2000) {
        lastMsg = now;
        temperature = readDHT();
        delay(100);
        char tempString[8];
        dtostrf(temperature, 1, 2, tempString);
        Serial.print("Temperature: ");
        Serial.println(tempString);
        client.publish("esp32/pre-heated-air", tempString);
    }
}