#include <ds18b20.h>

// Define and initialize the variables
OneWire oneWire(ONE_WIRE_BUS);
OneWire oneWire1(ONE_WIRE_BUS1);
OneWire oneWire2(ONE_WIRE_BUS2);
OneWire oneWire3(ONE_WIRE_BUS3);
DallasTemperature sensors(&oneWire);
DallasTemperature sensors1(&oneWire1);
DallasTemperature sensors2(&oneWire2);
DallasTemperature storageFluid(&oneWire3);

void setupDS(){
    sensors.begin();
    sensors1.begin();
    sensors2.begin();
    storageFluid.begin();
}

void readDS(){

    // call sensors.requestTemperatures() to issue a global temperature 
     // request to all devices on the bus
  // Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  sensors1.requestTemperatures();
  sensors2.requestTemperatures();
  storageFluid.requestTemperatures();
  // Serial.println("DONE");
  // After we got the temperatures, we can print them here.
  // We use the function ByIndex, and as an example get the temperature from the first sensor only.
  float tempC = sensors.getTempCByIndex(0);
  float tempC1 = sensors1.getTempCByIndex(0);
  float tempC2 = sensors2.getTempCByIndex(0);
  float sensorStorageFluid = storageFluid.getTempCByIndex(0);
  

  // Check if reading was successful
  if(tempC != DEVICE_DISCONNECTED_C) 
  {
    Serial.print("Device 1 Temp: ");
    Serial.print(tempC);
    Serial.print("\t Device 2 Temp: ");
    Serial.print(tempC1);
    Serial.print("\t Exchanger in ");
    Serial.print(tempC2);
    Serial.print("\t Storage Fluid ");
    Serial.print(sensorStorageFluid);
    Serial.print("\n");
  } 
  else
  {
    Serial.println("Error: Could not read temperature data\n");
  }

}