#define SEGLEN 4 // amount of segments
#define LEDLEN 8 // amount of LEDs per segment
#define BUTTON_DELAY 300 // button debounce time delay
#define COUNT_DELAY 1000 // time delay for count down
#define SEG_DELAY 1 // delay for segment shifts
#define BLINK_RATE 250 // rate for blinking
#define BLINK_DELAY 2000 // delay for blinking

enum states { // state machine states
  SET,
  COUNT,
  BLINK
};

static const uint8_t digitMap[] = { // digit map for segments
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
    0b01101111  // 9
};

//                          D1  D2 D3 D4
const uint8_t segments[] = {A5, 6, 7, 10}; // segment pins
//                      A  B  C   D   E   F  G
const uint8_t leds[] = {9, 5, A3, A1, 13, 8, A4}; // led pins

// button pins
const uint8_t btnY = 2;
const uint8_t btnB = 3;
const uint8_t btnR = 4;

// ones tens hundreds thousands
int numbers[] = {0, 0, 0, 0}; // initialize the number to 0

uint8_t current_state = SET; // initialize state machine to SET state
uint8_t btnY_state, btnB_state, btnR_state; // states of buttons

// timing variables
unsigned long setPast = 0;
unsigned long setNew = 0;
unsigned long countPast = 0;
unsigned long countNew = 0;
unsigned long ratePast = 0;
unsigned long rateNew = 0;
unsigned long blinkPast = 0;
unsigned long blinkNew = 0;

bool countingDone = false;
bool blinkDone = false;
uint8_t segsOn = HIGH; // determines if the segments are high or blinking

// SET UP FUNCTION ----------------------------------------------------------------
void setup() {
    uint8_t i; // iterator

    // set segments as output pins
    for (i = 0; i < SEGLEN; i++) {
        pinMode(segments[i], OUTPUT);
    }

    // set LEDs as output pins
    for (i = 0; i < LEDLEN; i++) {
        pinMode(leds[i], OUTPUT);
    }

    // set buttons as inputs
    pinMode(btnY, INPUT);
    pinMode(btnB, INPUT);
    pinMode(btnR, INPUT);

    Serial.begin(9600); // initialize serial communication
}

// LOOP --------------------------------------------------------------------------
void loop() {
  btnR_state = digitalRead(btnR); // read digital state of red button
  
  switch(current_state) { // state machine
    case SET:
      if (btnR_state) current_state = COUNT; // change to COUNT state on button press
      
      countSet(); // call state functions
      break;

    case COUNT:
      if (countingDone) current_state = BLINK; // change to BLINK after finish counting

      count(); // call state function
      break;

    case BLINK:
      if (blinkDone) current_state = SET; // change to SET after blinking for 2 seconds

      blinkSeg();
      break;
      
  }
  displaySeg();
  Serial.println(countingDone);
}

// SET STATE FUNCTION -------------------------------------------------------------
void countSet() {
  // read button states
  btnY_state = digitalRead(btnY); // yellow decrements
  btnB_state = digitalRead(btnB); // blue increments

  segsOn = HIGH;
//  blinkDone = false; // turn off flag for blinking state

  setNew = millis(); //get current time of the board
  if (btnY_state & (setNew - setPast > BUTTON_DELAY)) {
    setPast = setNew; // update the time
    decrement(); // decrement count
  }
  else if (btnB_state & (setNew - setPast > BUTTON_DELAY)) {
    setPast = setNew;
    increment(); // increment count
  }
}

// COUNT STATE FUNCTION -----------------------------------------------------------
void count() {
  segsOn = HIGH;
  bool finishFlag = true; // flag to see if we're done counting

  // check all the segments to see if they're not zero
  for (uint8_t i = 0; i < SEGLEN; i++) {
    if (numbers[i] > 0) {
      finishFlag = false;
    }
  }

  countNew = millis(); // get current board time
  if (!finishFlag & (countNew - countPast > COUNT_DELAY)) { // check if not done counting to count down
    countPast = countNew;
    decrement(); // decrement the count
    
  }

  // set flag to switch to the next state
  if (finishFlag) {
    countingDone = true;
  } else {
    countingDone = false;
  }
}

// BLINK STATE FUNCTION --------------------------------------------------------
void blinkSeg() {
  countingDone = false;
  uint8_t i; // iterator
  static uint8_t secCount = 0;

  rateNew = millis();
  blinkNew = millis();
  if (blinkNew - blinkPast > BLINK_DELAY) {
    blinkPast = blinkNew;
    secCount += 1;
  }
  
  if (rateNew - ratePast > BLINK_RATE) {
    ratePast = rateNew;
    if (segsOn == HIGH) {
      segsOn = LOW;
      
    } else {
      segsOn = HIGH;
    }
  }

  if (secCount == 2) {
    secCount = 0; // reset the seconds counter
    blinkDone = true; // set flag to exit state
  } else {
    blinkDone = false;
  }
}

void increment() {
  numbers[0] += 1; // increment ones place

  if (numbers[0] > 9) { // increment tens place
    numbers[1] += 1;
    numbers[0] = 0;
  }

  if (numbers[1] > 9) { // increment hundreds place
    numbers[2] += 1;
    numbers[1] = 0;
  }

  if (numbers[2] > 9) { // increment thousands place
    numbers[3] += 1;
    numbers[2] = 0;
  }

  if (numbers[3] > 9) {
    numbers[3] = 0;
  }
}

void decrement() {
  numbers[0] -= 1; // decrement ones place

  if (numbers[0] < 0) { // decrement tens place
    numbers[1] -= 1;
    numbers[0] = 9;
  }

  if (numbers[1] < 0) { // decrement hundreds place
    numbers[2] -= 1;
    numbers[1] = 9;
  }

  if (numbers[2] < 0) { // decrement thousands place
    numbers[3] -= 1;
    numbers[2] = 9;
  }

  if (numbers[3] < 0) {
    numbers[3] = 9;
  }
}

void displaySeg() {
  uint8_t i, j; // iterators

  if (segsOn) {
    for (i = 0; i < SEGLEN; i++) { // iterate through segments to decide which one to turn on
      displayNum(numbers[i]);
      for (j = 0; j < SEGLEN; j++) {
        if (j == i) {
          digitalWrite(segments[j], HIGH);
        } else {
          digitalWrite(segments[j], LOW);
        }
      }
      delay(SEG_DELAY);
    }
    
  } else {
    for (i = 0; i < SEGLEN; i++) {
      digitalWrite(segments[i], LOW);
    }
  }
}

void displayNum(uint8_t toDisplay) {
  uint8_t i; // iterator

  for (i = 0; i < LEDLEN; i++) { // iterate through bits
    if (digitMap[toDisplay] & (1 << i)) { // if the bit is high, turn on leg
      digitalWrite(leds[i], LOW);
      
    } else { // else turn it off
      digitalWrite(leds[i], HIGH);
    }
    
  }
}

void displayCount() {
  Serial.print("Segments: ");

  for(int i = 0; i < SEGLEN; i++) {
    Serial.print(numbers[i]);
    Serial.print(" ");
  }

//  Serial.print("countingDone: ");
//  Serial.println(countingDone);

  Serial.println("");
}
