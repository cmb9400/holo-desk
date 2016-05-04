#include <Wire.h>

#define xyzregister (0x32)
#define accel_module (0x53)

byte values[6];
byte base[3];
byte reading[3];

void setup() {
  Wire.begin();
  initAccel();
  cali();
  delay(200);
  pinMode(31, OUTPUT);
  pinMode(35, OUTPUT);
  for (int i = 2; i >= 0; i--) {
    digitalWrite(31, HIGH);
    delay(100); 
    digitalWrite(31, LOW);
    delay(100);
  }
}

void loop(){
  readAccel();
  int diff = abs(base[2]-reading[2]);
  if (diff > 3) { // lower to increase sensitivity hight to decrease
    tap();
    delay(200); // controlls the double tap thing high is less senistive
  }
}

void initAccel() {
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  delay(200);
}

void cali() {
  unsigned long temp[3] = { 0, 0, 0 };
  int loops = 1000;
  readAccel();
  while (!(reading[0] + reading[1] + reading[2])); //wait
  while (loops) { // read 1000 times and add to temp
    readAccel();
    temp[0] = reading[0] + temp[0];
    temp[1] = reading[1] + temp[1];
    temp[2] = reading[2] + temp[2];
    loops--;
  }
  // take an average of the data
  base[0] = (byte)(temp[0] / 1000);
  base[1] = (byte)(temp[1] / 1000);
  base[2] = (byte)(temp[2] / 1000);
}

void readAccel() {
  Wire.beginTransmission(accel_module);
  Wire.write(xyzregister);
  Wire.endTransmission();
  Wire.beginTransmission(accel_module);
  Wire.requestFrom(accel_module, 6);
  int i = 0;
  while(Wire.available()){
    values[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  reading[0] = (((int)values[1]) << 8) | values[0];
  reading[1] = (((int)values[3]) << 8) | values[2];
  reading[2] = (((int)values[5]) << 8) | values[4];
}

void tap() {
  digitalWrite(31, HIGH);
  digitalWrite(35, HIGH);
  delay(100); 
  digitalWrite(31, LOW);
  digitalWrite(35, LOW);
}


