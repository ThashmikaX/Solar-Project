#ifndef GLOBALS_H
#define GLOBALS_H

#include <WiFi.h>
#include <PubSubClient.h>

// Declare shared variables as extern
extern WiFiClient espClient;
extern PubSubClient client;
extern long lastMsg;
extern float temperature;
extern char msg[50];
extern int value;

void callback(char* topic, byte* payload, unsigned int length);
void setup_wifi();
void reconnect();

#endif // GLOBALS_H