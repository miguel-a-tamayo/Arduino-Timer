/*
 * Name: Miguel Tamayo
 * Date: 06/07/2022
 * 
 * fourDigit.c
 * Implementation file fot the fourDigit class
*/

#include "fourDigit.h"
#include "Arduino.h"

static const uint8_t digitMap[] = {
    // GFEDCBA
    0b00111111, // 0
    0b00000110, // 1
    0b01011011, // 2
    0b01001111, // 3
    0b01100110, // 4
    0b01101101, // 5
    0b01111101, // 6
    0b00000111, // 7
    0b01111111, // 8
    0b01101111  // 99
};

// Constructors ---------------------------------

//fourDigit()
// creates a new four digit display
fourDigit::fourDigit() {
    segmentOn = LOW;
    ledOn = LOW;
}

// display()
// displays each number on toDisplay to
// the corresponding segment
void fourDigit::begin(uint8_t commonNode, const uint8_t ledPinsIn[], const uint8_t segmentPinsIn[], uint8_t delay) {


    switch(commonNode) {
        case 0: // case of common cathode (ground)
            segmentOn = LOW;
            ledOn = HIGH;
            break;

        case 1: // case of common anode (vcc)
            segmentOn = HIGH;
            ledOn = LOW;
            break;
    }

    // set the off variables
    if (segmentOn == HIGH) {
        segmentOff = LOW;
    } else {
        segmentOff = HIGH;
    }

    if (ledOn == HIGH) {
        ledOff = LOW;
    } else {
        ledOn = HIGH;
    }

    // store the pins
    for (uint8_t i = 0; i < LEDS; i++) {
        ledPins[i] = ledPinsIn[i];
    }

    for (uint8_t i = 0; i < SEGS; i++) {
        segmentPins[i] = segmentPinsIn[i];
    }

    timeDelay = delay; // set the delay for the segments
}

// segmentOn()
// Turns on the specific segment with desired number
void fourDigit::segmentTurnOn(uint8_t segment, uint8_t number) {
    uint8_t i; // iterator

    // turn on the LEDs for the number
    for (i = 0; i < LEDS; i++) {

        if (digitMap[number] & (1 << i)) { // if bit is on, turn on LED
            digitalWrite(ledPins[i], ledOn);

        } else {
           digitalWrite(ledPins[i], ledOff); 
        }
    }

    digitalWrite(segmentPins[segment], segmentOn); // turn on segment
}

// segmentOff()
// Turns off specific segment
void fourDigit::segmentTurnOff(uint8_t segment) {
    uint8_t i; // iterator

    // turn on the LEDs for the number
    for (i = 0; i < LEDS; i++) {
        digitalWrite(ledPins[i], ledOff); 
    }

    digitalWrite(segmentPins[segment], segmentOff); // turn on segment
}

// display()
// displays each number on toDisplay to
// the corresponding segment
void fourDigit::display(uint8_t toDisplay[]) {
    uint8_t i, j; // iterators

    // iterate through the segments and turn them off / on
    for (i = 0; i < SEGS; i++) {
        for (j = 0; j < SEGS; j++) {
            if (j == i) { // check if it's the segment we want to turn on
                segmentTurnOn(j, toDisplay[i]); // display segment
            } else {
                segmentTurnOff(j);
            }

            delay(timeDelay);
        }
    }
}