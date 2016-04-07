#include <Wire.h>
#define xyzregister (0x32)
#define device0 (0x53)
#define device1 (0x1D)

byte values[6] ;
char output[512];
byte base[2][3];
byte reading[2][3];

void setup() {
  Wire.begin();
  Serial.begin(9600);
  init(device0);
  init(device1);
  readAccel(device0, 0);
  base[0][0] = reading[0][0];
  base[0][1] = reading[0][1];
  base[0][2] = reading[0][2];
  readAccel(device1, 1);
  base[1][0] = reading[1][0];
  base[1][1] = reading[1][1];
  base[1][2] = reading[1][2];
}

void loop() {
  readAccel(device0, 0);
  readAccel(device1, 1);
  int diff1 = abs(base[0][0]-reading[0][0]) + abs(base[0][1]-reading[0][1]) + abs(base[0][2]-reading[0][2]);
  int diff2 = abs(base[1][0]-reading[1][0]) + abs(base[1][1]-reading[1][1]) + abs(base[1][2]-reading[1][2]);
  if (diff1 > 30 || diff2 > 30) {
    sprintf(output, "Device 1: %d %d %d   Device 2: %d %d %d", reading[0][0], reading[0][1] ,reading[0][2], reading[1][0], reading[1][1] ,reading[1][2]);
    Serial.write(10); //New line
    Serial.print(output);
  }
  delay(200);
}

void init(byte device) {
  Wire.beginTransmission(device);
  Wire.write(0x2D);
  Wire.write(0);
  Wire.endTransmission();
  Wire.beginTransmission(device);
  Wire.write(0x2D);
  Wire.write(16);
  Wire.endTransmission();
  Wire.beginTransmission(device);
  Wire.write(0x2D);
  Wire.write(8);
  Wire.endTransmission();
  delay(200);
}

void readAccel(byte devive, int deviceNum) {
  Wire.beginTransmission(devive);
  Wire.write(xyzregister);
  Wire.endTransmission();
  Wire.beginTransmission(devive);
  Wire.requestFrom(devive, 6);
  int i = 0;
  while(Wire.available()){
    values[i] = Wire.read();
    i++;
  }
  Wire.endTransmission();
  reading[deviceNum][0] = (((int)values[1]) << 8) | values[0];
  reading[deviceNum][1] = (((int)values[3]) << 8) | values[2];
  reading[deviceNum][2] = (((int)values[5]) << 8) | values[4];
}
