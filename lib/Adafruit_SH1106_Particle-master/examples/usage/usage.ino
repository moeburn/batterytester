// Example usage for Adafruit_SH1106 library by some guy and me now I guess.

#include "Adafruit_SH1106.h"

// Initialize objects from the lib
Adafruit_SH1106 adafruit_SH1106;

void setup() {
    // Call functions on initialized library objects that require hardware
    adafruit_SH1106.begin();
}

void loop() {
    // Use the library's initialized objects and functions
    adafruit_SH1106.process();
}
