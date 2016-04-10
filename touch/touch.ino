#include <Wire.h>

#define xyzregister (0x32)
#define accel_module (0x53)

byte values[6];
char output[512];
byte base[3];
byte reading[3];

void setup() {
  Wire.begin();
  Serial.begin(9600);
  initAccel();
  cali();
  sprintf(output, "BASE: %d %d %d\n", base[0], base[1], base[2]);
  Serial.print(output);
  delay(200);
}

void loop(){
  readAccel();
  int diff = abs(base[0]-reading[0]) + abs(base[1]-reading[1]) + abs(base[2]-reading[2]);
  if (diff > 10) {
    sprintf(output, "%d %d %d\n", reading[0],reading[1] ,reading[2]);
    Serial.print(output);
    delay(200);
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
  Serial.println("Starting Calibration");
  byte last[3];
  int count;
  last[0] = 0;
  last[1] = 0;
  last[2] = 0;
  readAccel();

  while (true) {
    if (!(reading[0] == 0 || reading[1] == 0 || reading[2] == 0)) {
      if ((last[0] == reading[0] || last[1] == reading[1] || last[2] == reading[2])) {
        count++;
        if (count > 5) {
          base[0] = last[0];
          base[1] = last[1];
          base[2] = last[2];
          break;
        } else {
          readAccel();
          delay(200);
        }
      } else {
        last[0] = reading[0];
        last[1] = reading[1];
        last[2] = reading[2];
        count = 0;
        readAccel();
        delay(200);
      }
    } else {
      count = 0;
      readAccel();
      delay(200);
    }
  }
  Serial.println("Done Calibrating");
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
