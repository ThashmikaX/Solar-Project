#include <Arduino.h>
#include <dht_11.h>
#include <flow.h>
#include <ds18b20.h>
#include <Wire.h>
#include <WiFi.h> //included in ESP32 boards package
#include <PubSubClient.h> //Install with Arduino Library manager or download at https://github.com/knolleary/pubsubclient
#include "Credentials.h"
#include "AnalogReader.h"


WiFiClient espClient;
PubSubClient client(espClient);// To connect more ESP32's change "client" to for instance "client2" here and in the rest of the code
long lastMsg = 0;
char msg[50];
int value = 0;
float temperature = 0;
const int ledPin = 2;//connect LED to GPIO pin 2
const int potmeterPin = 34;  //Connect Potentiometer to GPIO34

void setup_wifi();
void callback(char* topic, byte* message, unsigned int length);

// Create an instance of the AnalogReader class
AnalogReader analogReader(34);

void setup()
{
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);//set ledPin to output
  //Potentiometer GPIO34: no Pinmode for Analog-inputs: The analogRead() function takes care of that :-)

  setupDHT();
  delay(5500);
  setupDS();
  flowSetup();
}

void setup_wifi() {  // Connect to WiFi network
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

void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "esp32/output") {//Check if the MQTT message matches the subscription, if add more ESP32 change topic esp32 to for instance esp33
    Serial.print("Changing output to ");
    if(messageTemp == "true"){//if the message is "true", switch on
      Serial.println("on");
      digitalWrite(ledPin, HIGH);
    }
    else if(messageTemp == "false"){//if the message is "false", switch off
      Serial.println("off");
      digitalWrite(ledPin, LOW);
    }
  }  // You can add more "if" statements behind this to control more GPIOs with MQTT
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

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long now = millis();
  if (now - lastMsg > 2000) { // message refresh rate in milliseconds
    lastMsg = now;
    // int analogValue = analogRead(potmeterPin);//read analog GPIO 34 - the potentiometer
    // temperature = (analogValue/40);//12 bit: 0-4096 make it "more-or-less" 0-100

    // Read temperatures from DS18B20 sensors
    float* lTemperatures = readDS();

    // Create strings to hold the temperature values
    char liquidTempString[4][8];
    for (int i = 0; i < 4; i++) {
        dtostrf(lTemperatures[i], 6, 2, liquidTempString[i]);
    }
    
    
    // Assuming you have a function that reads all DHT sensor values like readAllDHTSensors()
// and stores the results in a struct (SensorReadings).

SensorReadings sensorData = readAllDHTSensors(); // Read temperature and humidity from both sensors

// Publish D-Chamber (DHT1) temperature and humidity data
char temp1String[8], hum1String[8];
dtostrf(sensorData.temp1, 1, 2, temp1String); // Convert temp1 to string
dtostrf(sensorData.humidity1, 1, 2, hum1String); // Convert humidity1 to string
delay(2000);  // Delay between readings

Serial.print("D-Chamber Temperature: ");
Serial.println(temp1String);
Serial.print("D-Chamber Humidity: ");
Serial.println(hum1String);

// Publish temperature and humidity for D-Chamber
client.publish("esp32/d-chamber/temperature", temp1String);
client.publish("esp32/d-chamber/humidity", hum1String);

// Publish Ambient Air (DHT2) temperature and humidity data
char temp2String[8], hum2String[8];
dtostrf(sensorData.temp2, 1, 2, temp2String); // Convert temp2 to string
dtostrf(sensorData.humidity2, 1, 2, hum2String); // Convert humidity2 to string

Serial.print("Ambient Air Temperature: ");
Serial.println(temp2String);
Serial.print("Ambient Air Humidity: ");
Serial.println(hum2String);

// Publish temperature and humidity for Ambient Air
client.publish("esp32/ambient-air/temperature", temp2String);
client.publish("esp32/ambient-air/humidity", hum2String);
// Assuming you have liquid temperature readings in the array liquidTempString[]
Serial.print("Liquid Sensor 1: ");
Serial.println(lTemperatures[0]);
Serial.print("Liquid Sensor 2: ");
Serial.println(lTemperatures[1]);
Serial.print("Liquid Sensor 3: ");
Serial.println(lTemperatures[1]);
Serial.print("Liquid Sensor 4: ");
Serial.println(lTemperatures[1]);

// Assuming you have liquid temperature readings in the array liquidTempString[]
Serial.print("Liquid Sensor 1: ");
Serial.println(liquidTempString[0]);
Serial.print("Liquid Sensor 2: ");
Serial.println(liquidTempString[1]);
Serial.print("Liquid Sensor 3: ");
Serial.println(liquidTempString[2]);
Serial.print("Liquid Sensor 4: ");
Serial.println(liquidTempString[3]);

// Publish liquid temperature sensor values
client.publish("esp32/solar-collector-in", liquidTempString[0]);
client.publish("esp32/solar-collector-out", liquidTempString[1]);
client.publish("esp32/exchanger-in", liquidTempString[2]);
client.publish("esp32/storage-fluid", liquidTempString[3]);

float flowRate = readFlow();
char flowString[8];
dtostrf(flowRate, 1, 2, flowString);
client.publish("esp32/flow-rate", flowString);
Serial.print(flowRate);
Serial.print("\n");

float heater = analogReader.readValue();
char heaterString[8];
dtostrf(heater, 1, 2, heaterString);
client.publish("esp32/heater", heaterString);

  // Print the result to the Serial Monitor
Serial.print("Result: ");
Serial.println(heater);


// Delay for 100 ms to allow clear reads and transmissions
delay(500);

  }
}