#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <dht_11.h>
#include <flow.h>
#include <ds18b20.h>
#include <Wire.h>

// WiFi credentials
const char* ssid = "your_SSID";
const char* password = "your_PASSWORD";

// Google Apps Script URL
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycby8e4LqBUUa_RUNLIXSS559z_eB4k4bcHzbIXpCyIHV6dJu-hmobhuMSbU-bThftBTf/exec";

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi");
  setupDHT();
  setupDS();
  setupFlow();
}

void loop() {
  float dhtValue = readDHT();
  float flowValue = readFlow();
  float* dsValues = readDS();

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(googleScriptURL);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"dht\":" + String(dhtValue) + 
                         ",\"flow\":" + String(flowValue) + 
                         ",\"ds\":[" + String(dsValues[0]) + 
                         "," + String(dsValues[1]) + 
                         "," + String(dsValues[2]) + 
                         "," + String(dsValues[3]) + "]}";

    int httpResponseCode = http.POST(jsonPayload);

    if (httpResponseCode > 0) {
      String response = http.getString();
      Serial.println(httpResponseCode);
      Serial.println(response);
    } else {
      Serial.print("Error on sending POST: ");
      Serial.println(httpResponseCode);
    }

    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  delay(2000); // Send data every 60 seconds
}