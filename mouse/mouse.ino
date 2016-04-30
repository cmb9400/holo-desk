#include <Mouse.h>

enum states {
  NO_CLICK,
  CLICK,
  HOLD,
  DRAG,
  RELEASE,
};

enum states state = NO_CLICK;

void setup() {
  Serial.begin(9600);
  Mouse.begin();
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING);
}

void loop() {
  switch(state) {
    case CLICK:
      Mouse.click();
      state = NO_CLICK;
      break;
    case HOLD:
      Mouse.press();
      state = DRAG;
      break;
    case RELEASE:
      Mouse.release();
      state = NO_CLICK;
      break;
  }
}

void interrupt() {
  detachInterrupt(digitalPinToInterrupt(2));
  switch(state) {
    case NO_CLICK:
      state = CLICK;
      break;
    case CLICK:
      state = HOLD;
      break;
    case DRAG:
      state = RELEASE;
  }
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING);
}

