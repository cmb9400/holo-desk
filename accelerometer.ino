#include <Wire.h>
#define accel_module (0x53)
#define xyzregister (0x32)

byte values[6] ;
char output[512];
byte base[3];
byte reading[3];

void setup(){
  Wire.begin();
  Serial.begin(9600);
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
  readAccel();
  base[0] = reading[0];
  base[1] = reading[1];
  base[2] = reading[2];
}

void loop(){
  readAccel();
  int diff = abs(base[0]-reading[0]) + abs(base[1]-reading[1]) + abs(base[2]-reading[2]);
  if (diff > 30) {
    sprintf(output, "%d %d %d", reading[0],reading[1] ,reading[2]);
    Serial.print(output);
    Serial.write(10);
    //This is where we can change the interval
    delay(200);
  }
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
