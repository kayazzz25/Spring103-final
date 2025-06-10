#include <Adafruit_CircuitPlayground.h>

// GLOBAL VARIABLES
const unsigned long hardMode = 1000; //to the left
const unsigned long normalMode = 3000; //to the right
//these are for the delay range before the led turns on
const int minDelay = 2000; 
const int maxDelay = 5000; 

const int DEBOUNCE_DELAY = 50;//avoid false triggers

// use enum instead of string, more reliable, gets rid of the warning message (costum constant)
enum ButtonType { LEFT, RIGHT }; //defines a set of named constant values, comtom data tpye

// --- State Variables ---
unsigned long timerStart; //when the leds turn on
unsigned long delayTimerStart; //timestamp
unsigned long delayDuration; //min and max
unsigned long reactionTimeLimit;

bool waitingForReaction = false;
bool waitingForDelay = false;

//declaring a variable within type
ButtonType expectedButton; //expectedbutton can store a value of buttontype (left or right)

void setup() {
  Serial.begin(9600);
  CircuitPlayground.begin();
  randomSeed(analogRead(A0));
  delay(2000);
}

void loop() {
  // Update difficulty mode
  if (CircuitPlayground.slideSwitch()) {
  reactionTimeLimit = hardMode; //left
} else {
  reactionTimeLimit = normalMode; //right
}

  // Start a new round if we're not waiting for anything, determines delay
  if (!waitingForDelay && !waitingForReaction) { //!=not, && = both be true
    delayDuration = random(minDelay, maxDelay); //picks a delay
    delayTimerStart = millis(); //this is the timestamp (records the time)
    waitingForDelay = true; //wait for delay
  }

//delay is done
  if (waitingForDelay && millis() - delayTimerStart >= delayDuration) { //if waitfordealy and (millis() - delayTimerStart) the time thats passed since delay started is true then run
    // Randomly choose color and expected button
    if (random(0, 2)) { //choses a color is displayed
      setAllPixels(255, 255, 0); //yellow 1
      expectedButton = RIGHT;
    } else {
      setAllPixels(0, 0, 255); //blue 0
      expectedButton = LEFT;
    }

    timerStart = millis();
    waitingForReaction = true; //waiting for a reaction
    waitingForDelay = false; //not waiting for random delay
  }

  // Handle button input
  if (waitingForReaction) {
    unsigned long currentTime = millis();

    // Check for timeout
    if (currentTime - timerStart > reactionTimeLimit) { //time it take > limit = too slow 
      Serial.println("Too slow!");
      clearAllPixels();
      failPattern();
      failTone();
      waitingForReaction = false;
      delay(2000);
      return;
    }

    // Debounced button press detection
    if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()) { //check if button is pressed
      delay(DEBOUNCE_DELAY); // Simple debounce
      currentTime = millis(); // Update current time post-debounce
      unsigned long reactionTime = currentTime - timerStart; //records reaction time
      clearAllPixels();
      waitingForReaction = false;

      bool correct = false; //flag for when a correct button is pressed
      if (expectedButton == LEFT && CircuitPlayground.leftButton()) {
        correct = true;
      } else if (expectedButton == RIGHT && CircuitPlayground.rightButton()) {
        correct = true;
      }

      if (correct && reactionTime <= reactionTimeLimit) {
        Serial.print("Yup! you got it!! :) ");
        smileyPattern();
        celebrationTone();
      } else if (correct && reactionTime > reactionTimeLimit) {
        Serial.print("Too slow, Too bad. ");
        failPattern();
        failTone();
      } else {
        Serial.println("Nope, wrong button dumb dumb.");
        failPattern();
        failTone();
      }

      delay(1000); // Short pause before next round
    }
  }
}

//this is used turn on all the leds, one color
void setAllPixels(uint8_t r, uint8_t g, uint8_t b) { //uint8_t is for color values
  for (int i = 0; i < 10; i++) { //turns on all led
    CircuitPlayground.setPixelColor(i, r, g, b);
  }
}

void clearAllPixels() {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
}

void celebrationTone() {
    CircuitPlayground.playTone(660, 200); //high
    delay(250);
    CircuitPlayground.playTone(880, 200); //higher
}

void failTone() {
  CircuitPlayground.playTone(330, 200); //low
  delay(250);
  CircuitPlayground.playTone(220, 300); //lower
}

void smileyPattern() {
  clearAllPixels();
  uint32_t purple = CircuitPlayground.strip.Color(255, 0, 255);

  CircuitPlayground.setPixelColor(0, purple); // Left eye
  CircuitPlayground.setPixelColor(2, purple); // Right eye
  CircuitPlayground.setPixelColor(3, purple); // Mouth
  CircuitPlayground.setPixelColor(4, purple);
  CircuitPlayground.setPixelColor(5, purple);
  CircuitPlayground.setPixelColor(6, purple);
  CircuitPlayground.setPixelColor(7, purple);
  CircuitPlayground.setPixelColor(9, purple);
}

void failPattern() {
  setAllPixels(255, 0, 0); // All red
}