#include "HID-Project.h"
#include <AltSoftSerial.h>

//  See HID Project documentation for more infos
//  https://github.com/NicoHood/HID/wiki/Mouse-API
//  https://github.com/NicoHood/HID/wiki/AbsoluteMouse-API

// See AltSoftSerial for more info
// http://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
// https://github.com/PaulStoffregen/AltSoftSerial

AltSoftSerial mySerial;
int missedPackets = 0;

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
  mySerial.begin(9600);
  Serial.begin(115200);    // serial / USB port
  while (!Serial);
  AbsoluteMouse.begin();
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING); //interrupt pin from Mega
  Serial.println("Ready");
}

void clickMouse(){
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
}

void loop() {

  clickMouse();

  // commands from the pi terminate with '.'
  char cmd[11] = "";
  char cmd2[11] = ""; // sse parity \o/

  // check if byte available on the software serial port
  //receives zero-padded string of 16 bit coordinates (xy) ending with a '.'
  if (mySerial.available()) {
    mySerial.readBytesUntil('.', cmd, 11);
    clickMouse();
    while (!mySerial.available()) {}
    mySerial.readBytesUntil('.', cmd2, 11);

    String cmdStr = String(cmd);
    String cmdStr2 = String(cmd2);

    if (cmdStr.equals(cmdStr2)) {
      //mySerial.println(cmd);
      //Serial.println("cmd: " + cmdStr);
      String dx = cmdStr.substring(0,5);
      String dy = cmdStr.substring(5,10);
      //Serial.println("goto " + dx + " " + dy);
      int x = dx.toInt();
      int y = dy.toInt();
      AbsoluteMouse.moveTo(x,y);
    } else {
      missedPackets++;
      if (missedPackets >= 15){
        missedPackets = 0;
        mySerial.readBytesUntil('.', cmd, 11);
        loop();  
      }
      //Serial.println("Packet miss");      
    }
  }
  
}
