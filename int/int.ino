int yolo = 0;
void setup() {
  Serial.begin(9600);
  attachInterrupt(0, swag, RISING);
}

void loop() {
  delay(1000);
}

void swag() {
  yolo++;
  Serial.println(yolo);
}
