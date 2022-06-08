/*
* fourDigit.h
* header file for 4 digit 7-segment display
*/

#ifndef fourDigit_h
#define fourDigit_h

#include "Arduino.h"

#define LEDS 8
#define SEGS 4

class fourDigit
{
private:
    uint8_t segmentOn; // segment on variable
    uint8_t ledOn; // LED on variable
    uint8_t segmentOff; // segment off variable
    uint8_t ledOff; // LED off variable
    uint8_t ledPins[8]; // pins for the LEDs [DP G F E D C B A]
    uint8_t segmentPins[4]; // pins for the segments [D4 D3 D2 D1]
    uint8_t timeDelay;

    // Manipulation Functions -------------------

public:
    // Constructor ------------------------------
    // creates a new four digit display
    fourDigit();

    // Access Functions -------------------------

    // Manipulation Functions -------------------
    
    // begin()
    // sets up the 4 digi 7seg display to specific pins
    void begin(uint8_t commonNode, const uint8_t ledPins[], const uint8_t segmentPins[], uint8_t delay);

    // segmentOn()
    // Turns on the specific segment with desired number
    void segmentTurnOn(uint8_t segment, uint8_t number);

    // segmentOff()
    // Turns off specific segment
    void segmentTurnOff(uint8_t segment);

    // display()
    // displays each number on toDisplay to
    // the corresponding segment
    void display(uint8_t toDisplay[]);


};

#endif