#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <dht_11.h>
#include <flow.h>
#include <ds18b20.h>
#include <Wire.h>

// WiFi credentials
const char* ssid = "Dialog 4G 376";
const char* password = "Enigmas1";

// Google Apps Script URL
const char* googleScriptURL = "https://script.google.com/macros/s/AKfycbyJ2NxwW6h5phGAmb0doewDHxM3juDeM_LC4S2doJcqZsHUKAljQxT73SEJh34_AsR5/exec";

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
  // Call the function to read data from both sensors
  SensorReadings readings = readAllDHTSensors();

  // Access and use the individual values
  float temp1 = readings.temp1;
  float temp2 = readings.temp2;
  float hu1 = readings.humidity1;
  float hu2 = readings.humidity2;
  delay(2000);
  float flowValue = readFlow();
  delay(1200);
  float* dsValues = readDS();
  delay(200);

  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    http.begin(googleScriptURL);
    http.addHeader("Content-Type", "application/json");

    String jsonPayload = "{\"dht1\": {\"temperature\": " + String(temp1) + 
                    ", \"humidity\": " + String(hu1) + "}," +
                    "\"dht2\": {\"temperature\": " + String(temp2) + 
                    ", \"humidity\": " + String(hu2) + "}," +
                    "\"flow\": " + String(flowValue) + "," +
                    "\"ds\": [" + String(dsValues[0]) + 
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