#include <WiFi.h> //included in ESP32 boards package
#include <PubSubClient.h> //Install with Arduino Library manager or download at https://github.com/knolleary/pubsubclient
#include "Credentials.h"
#include "dht_11.h"

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

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(MQTT_SERVER, 1883);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);//set ledPin to output
  //Potentiometer GPIO34: no Pinmode for Analog-inputs: The analogRead() function takes care of that :-)

  setupDHT();
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
    
    temperature = readDHT();
    delay(100);  // delay in between reads for clear read from serial
    char tempString[8];
    dtostrf(temperature, 1, 2, tempString);
    Serial.print("Temperature: ");
    Serial.println(tempString);
    client.publish("esp32/pre-heated-air", tempString);//send the message to the MQTT broker, if add more ESP32 change topic esp32 to for instance esp33
  }
}