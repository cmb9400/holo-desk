#include <Mouse.h>

boolean click = false;

void setup() {
  Serial.begin(9600);
  attachInterrupt(2, interrupt, RISING);
}

void loop() {
  if (click) {
    Mouse.click();
    click = false;
  }
}

void interrupt() {
  detachInterrupt(2);
  Serial.println("swag is go");
  click = true;
  attachInterrupt(2, interrupt, RISING);
}

