#include <Adafruit_CircuitPlayground.h>

// GLOBAL VARIABLES
const unsigned long hardMode = 1000; //to the left
const unsigned long normalMode = 3000; //to the right
//these are for the delay range before the led turns on
const int minDelay = 2000; 
const int maxDelay = 5000; 

const int DEBOUNCE_DELAY = 50;

// use enum instead of string, more reliable, gets rid of the warning message (costum constant)
enum ButtonType { LEFT, RIGHT };

// --- State Variables ---
unsigned long timerStart; //when the leds turn on
unsigned long delayTimerStart;
unsigned long delayDuration;
unsigned long reactionTimeLimit;

bool waitingForReaction = false;
bool waitingForDelay = false;

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

  // Start a new round if we're not waiting for anything
  if (!waitingForDelay && !waitingForReaction) { //!=not, && = both be true
    delayDuration = random(minDelay, maxDelay);
    delayTimerStart = millis();
    waitingForDelay = true;
  }

  // Handle non-blocking delay before signal
  if (waitingForDelay && millis() - delayTimerStart >= delayDuration) {
    // Randomly choose color and expected button
    if (random(0, 2)) {
      setAllPixels(255, 255, 0); //yellow
      expectedButton = RIGHT;
    } else {
      setAllPixels(0, 0, 255); //blue
      expectedButton = LEFT;
    }

    timerStart = millis();
    waitingForReaction = true;
    waitingForDelay = false;
  }

  // Handle button input
  if (waitingForReaction) {
    unsigned long currentTime = millis();

    // Check for timeout
    if (currentTime - timerStart > reactionTimeLimit) { //time it take > limit = too slow 
      Serial.println("Too slow!");
      clearAllPixels();
      failTone();
      waitingForReaction = false;
      delay(2000);
      return;
    }

    // Debounced button press detection
    if (CircuitPlayground.leftButton() || CircuitPlayground.rightButton()) {
      delay(DEBOUNCE_DELAY); // Simple debounce
      currentTime = millis(); // Update current time post-debounce
      unsigned long reactionTime = currentTime - timerStart;
      clearAllPixels();
      waitingForReaction = false;

      bool correct = false;
      if (expectedButton == LEFT && CircuitPlayground.leftButton()) {
        correct = true;
      } else if (expectedButton == RIGHT && CircuitPlayground.rightButton()) {
        correct = true;
      }

      if (correct && reactionTime <= reactionTimeLimit) {
        Serial.print("Yup! you got it!! :) ");
        celebrationTone();
      } else if (correct && reactionTime > reactionTimeLimit) {
        Serial.print("Too slow, Too bad. ");
        failTone();
      } else {
        Serial.println("Nope, wrong button dumb dumb.");
        failTone();
      }

      delay(1000); // Short pause before next round
    }
  }
}

void setAllPixels(uint8_t r, uint8_t g, uint8_t b) {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, r, g, b);
  }
}

void clearAllPixels() {
  for (int i = 0; i < 10; i++) {
    CircuitPlayground.setPixelColor(i, 0, 0, 0);
  }
}

void celebrationTone() {
    CircuitPlayground.playTone(660, 200);
    delay(250);
    CircuitPlayground.playTone(880, 200);
}

void failTone() {
  CircuitPlayground.playTone(330, 200);
  delay(250);
  CircuitPlayground.playTone(220, 300);
}
