#include "AnalogReader.h"
#include <Arduino.h>

AnalogReader::AnalogReader(int pin) : _pin(pin) {
    pinMode(_pin, INPUT); // Set the pin as input
}

int AnalogReader::readValue() {
    int analogValue = analogRead(_pin); // Read the analog value

    // Check thresholds and return appropriate values
    if (analogValue >= 0 && analogValue <= 1500) {
        return 0; // Return 0 for values between 0 and 1500
    } else if (analogValue > 3000) {
        return 1; // Return 1 for values greater than 3000
    }
    
    return -1; // Optional: Return -1 for values between 1501 and 3000 (if needed)
}