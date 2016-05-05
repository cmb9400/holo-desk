#include "HID-Project.h"
#include <AltSoftSerial.h>

//  See HID Project documentation for more infos
//  https://github.com/NicoHood/HID/wiki/Mouse-API
//  https://github.com/NicoHood/HID/wiki/AbsoluteMouse-API

// See AltSoftSerial for more info
// http://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
// https://github.com/PaulStoffregen/AltSoftSerial
AltSoftSerial mySerial;

// Number of corrupt
int missedPackets = 0;

// commands from the pi terminate with '.'
char cmd[11] = "";
char cmd2[11] = ""; // sse parity \o/

enum states {
  NO_CLICK,
  CLICK,
  HOLD,
  DRAG,
  RELEASE,
};

enum serial_states {
  NO_DATA,
  DATA_ONE,
  DATA_TWO,
}

enum states state = NO_CLICK;

enum serial_states serial_state = NO_DATA;

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

void processData() {
  String cmdStr = String(cmd);
  String cmdStr2 = String(cmd2);
  if (cmdStr.equals(cmdStr2)) {
    String dx = cmdStr.substring(0,5);
    String dy = cmdStr.substring(5,10);
    int x = dx.toInt();
    int y = dy.toInt();
    AbsoluteMouse.moveTo(x,y);
  } else {
    missedPackets++;
    if (missedPackets >= 10){
      missedPackets = 0;
      mySerial.readBytesUntil('.', cmd, 11);
      serial_state = NO_DATA; // reset back to first
      return; // return here so that serial_state is not set to NO_DATA
    }
  }
  serial_state = NO_DATA;
}


void setup() {
  mySerial.begin(9600);
  AbsoluteMouse.begin();
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING); //interrupt pin from Mega
}



void loop() {
  if (state == CLICK) {
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
  switch(serial_state) {
    case NO_DATA:
      // check if byte available on the software serial port
      //receives zero-padded string of 16 bit coordinates (xy) ending with a '.'
      if (mySerial.available()) {
        mySerial.readBytesUntil('.', cmd, 11);
        serial_state = DATA_ONE;
      }
      break;
    case DATA_ONE:
      // check if byte available on the software serial port
      //receives zero-padded string of 16 bit coordinates (xy) ending with a '.'
      if (mySerial.available()) {
        mySerial.readBytesUntil('.', cmd2, 11);
        serial_state = DATA_TWO;
      }
      break;
    case DATA_TWO: 
      processData();
      break;
  }
}
