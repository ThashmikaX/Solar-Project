#include "globals.h"
#include "credentials.h"
#include <Arduino.h>

// Define shared variables
WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
float temperature = 0.0;
char msg[50];
int value = 0;

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Message arrived on topic: ");
    Serial.print(topic);
    Serial.print(". Message: ");
    String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
    messageTemp += (char)payload[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {//Check if the MQTT message matches the subscription, if add more ESP32 change topic esp32 to for instance esp33
    Serial.print("Changing output to ");
    if(messageTemp == "true"){//if the message is "true", switch on
      Serial.println("on");
    }
    else if(messageTemp == "false"){//if the message is "false", switch off
      Serial.println("off");
    }
  }  // You can add more "if" statements behind this to control more GPIOs with MQTT
}

void setup_wifi() {
    delay(100);
  Serial.println();
  Serial.print("Connecting to Wifi");
  //Serial.println(SSID); //uncomment if you want to see your SSID in serial monitor
  WiFi.begin(SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
    while (!client.connected()) {   // Loop until connected to the MQTT server
    Serial.print("Trying MQTT connection...");
    if (client.connect("ESP8266Client")) {     // Try to connect
      Serial.println("connected");
      client.subscribe("esp32/output");      // Subscribe to topic, if add more ESP32 change topic esp32 to for instance esp33
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);      // Wait 5 seconds before retrying
    }
  }
}