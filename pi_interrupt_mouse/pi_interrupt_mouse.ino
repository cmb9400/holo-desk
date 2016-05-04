#include "HID-Project.h"
#include <AltSoftSerial.h>

//  See HID Project documentation for more infos
//  https://github.com/NicoHood/HID/wiki/Mouse-API
//  https://github.com/NicoHood/HID/wiki/AbsoluteMouse-API

// See AltSoftSerial for more info
// http://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
// https://github.com/PaulStoffregen/AltSoftSerial

AltSoftSerial mySerial;

enum states {
  NO_CLICK,
  CLICK,
  HOLD,
  DRAG,
  RELEASE,
};

enum states state = NO_CLICK;

void interrupt() {
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
}

void setup() {
  mySerial.begin(115200);
  Serial.begin(115200, SERIAL_8E1);    // serial / USB port
  while (!Serial);
  AbsoluteMouse.begin();
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING); //interrupt pin from Mega
}

void loop() {
  if (state == CLICK){
    delay(500); //delay so if there's another interrupt it will change state
  }
  
  switch(state) {
    case CLICK:
      AbsoluteMouse.click();
      state = NO_CLICK;
      break;
    case HOLD:
      AbsoluteMouse.press();
      state = DRAG;
      break;
    case RELEASE:
      AbsoluteMouse.release();
      state = NO_CLICK;
      break;
  }

  String cmd = "";
  //command terminates with '.'

  // check if byte available on the software serial port
  //receives zero-padded string of 16 bit coordinates (xy) ending with a '.'
  if (mySerial.available()) {
    cmd = (mySerial.readStringUntil('.'));
    mySerial.println(cmd);
    Serial.println(cmd);
    String dx = cmd.substring(0,5);
    String dy = cmd.substring(5,10);
    Serial.println(dx + " " + dy);
    int x = dx.toInt();
    int y = dy.toInt();
    AbsoluteMouse.moveTo(x,y);
  }
  
}
