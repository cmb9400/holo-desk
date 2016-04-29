#include <Mouse.h>

boolean click = false;

void setup() {
  Serial.begin(9600);
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING);
}

void loop() {
  if (click) {
    Mouse.click();
    click = false;
  }
}

void interrupt() {
  detachInterrupt(digitalPinToInterrupt(2));
  click = true;
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING);
}

