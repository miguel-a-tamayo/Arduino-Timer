#include "fourDigit.h"

uint8_t segments[] = {A5, 6, 7, 10}; // segment pins
uint8_t leds[] = {A2, A4, 8, 13, A1, A3, 5, 9}; // led pins

// fourDigit display; // isntance of the 4 digit display class

void setup() {
    // initiate the display with common cathode and 1s delay
    // display.begin(0, leds, segments, 1000);
}

void loop() {
    // display.segmentTurnOn(0, 0);
}