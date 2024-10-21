#include "dht_11.h"
#include "dht_11.h"

// Initialize DHT sensors
DHT dht(DHTPIN, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

void setupDHT() {
  Serial.println(F("DHTxx test!"));
  dht.begin();
  dht2.begin();
  delay(2000);  // Initial delay after setup
}

SensorReadings readAllDHTSensors() {
  SensorReadings data;

  // Read data from Sensor 1
  data.humidity1 = dht.readHumidity();
  data.temp1 = dht.readTemperature();

  // Read data from Sensor 2
  data.humidity2 = dht2.readHumidity();
  data.temp2 = dht2.readTemperature();

  // Check if any reads failed for Sensor 1
  if (isnan(data.humidity1) || isnan(data.temp1)) {
    Serial.println(F("Failed to read from Sensor 1"));
    data.humidity1 = 0.0;
    data.temp1 = 0.0;
  }

  // Check if any reads failed for Sensor 2
  if (isnan(data.humidity2) || isnan(data.temp2)) {
    Serial.println(F("Failed to read from Sensor 2"));
    data.humidity2 = 0.0;
    data.temp2 = 0.0;
  }

  // Print the results for both sensors
  Serial.print(F("Sensor 1 - Humidity: "));
  Serial.print(data.humidity1);
  Serial.print(F("%  Temperature: "));
  Serial.println(data.temp1);

  Serial.print(F("Sensor 2 - Humidity: "));
  Serial.print(data.humidity2);
  Serial.print(F("%  Temperature: "));
  Serial.println(data.temp2);

  return data;  // Return the data struct with all values
}
