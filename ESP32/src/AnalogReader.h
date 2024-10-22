#ifndef ANALOGREADER_H
#define ANALOGREADER_H

class AnalogReader {
public:
    AnalogReader(int pin); // Constructor
    int readValue(); // Method to read and process the analog value

private:
    int _pin; // Pin number for analog reading
};

#endif // ANALOGREADER_H