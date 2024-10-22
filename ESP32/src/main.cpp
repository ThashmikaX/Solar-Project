#include <Arduino.h>
#include <Wire.h>

// Define the pin for analog reading
const int analogPin = 34; // GPIO 12

void setup() {
  // Start the Serial communication for debugging
  Serial.begin(115200);
  
  // Set the analog pin as input (not necessary, but good practice)
  pinMode(analogPin, INPUT);
}

void loop() {
  // Read the analog value from pin 12
  int analogValue = analogRead(analogPin);
  
  // Print the value to the Serial Monitor
  Serial.print("Analog Value at Pin 12: ");
  Serial.println(analogValue);
  
  // Wait for a short period before the next reading
  delay(500); // Delay in milliseconds
}
