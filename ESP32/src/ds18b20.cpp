#include <ds18b20.h>

// Define and initialize the variables
OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire1(ONE_WIRE_BUS1);
DallasTemperature sensors(&oneWire);
DallasTemperature sensors1(&oneWire1);

void setupDS(){
    sensors.begin();
    sensors1.begin();
}

void readDS(){

    // call sensors.requestTemperatures() to issue a global temperature 
     // request to all devices on the bus
  // Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  sensors1.requestTemperatures();
  // Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);
  float tempC1 = sensors1.getTempCByIndex(0);

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Device 1 Temp: ");
    Serial.print(tempC);
    Serial.print("\t Device 2 Temp: ");
    Serial.print(tempC1);
    Serial.print("\n");
  } 
  else
  {
    Serial.println("Error: Could not read temperature data\n");
  }

}