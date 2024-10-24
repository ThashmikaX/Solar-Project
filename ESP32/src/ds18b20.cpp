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

float* readDS() {
    sensors.begin();
    sensors1.begin();
    sensors2.begin();
    storageFluid.begin();

    static float temperatures[4]; // Array to hold the temperature values

    // Call sensors.requestTemperatures() to issue a global temperature 
    // request to all devices on the bus
    sensors.requestTemperatures(); // Send the command to get temperatures
    sensors1.requestTemperatures();
    sensors2.requestTemperatures();
    storageFluid.requestTemperatures();

    // Get the temperature readings
    temperatures[0] = sensors.getTempCByIndex(0);
    temperatures[1] = sensors1.getTempCByIndex(0);
    temperatures[2] = sensors2.getTempCByIndex(0);
    temperatures[3] = storageFluid.getTempCByIndex(0);


    // Check if reading was successful
    if (temperatures[0] == DEVICE_DISCONNECTED_C) {
        Serial.println("Error: Could not read temperature data\n");
    } else {
        Serial.print("Device 1 Temp: ");
        Serial.print(temperatures[0]);
        Serial.print("\t Device 2 Temp: ");
        Serial.print(temperatures[1]);
        Serial.print("\t Exchanger in ");
        Serial.print(temperatures[2]);
        Serial.print("\t Storage Fluid ");
        Serial.print(temperatures[3]);
        Serial.print("\n");
    }

    return temperatures;
}