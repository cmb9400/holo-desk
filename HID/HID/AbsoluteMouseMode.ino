#include "HID-Project.h"
//  See HID Project documentation for more infos
//  https://github.com/NicoHood/HID/wiki/Mouse-API
//  https://github.com/NicoHood/HID/wiki/AbsoluteMouse-API

void setup() {
  AbsoluteMouse.begin();

  Serial.begin(9600);    // serial / USB port
  Serial1.begin(9600);   // Arduino RX - white, Arduino TX - green
  
  delay(2000);
  Serial.print("starting");
}



void loop() {
  String cmd = "";
  //command terminates with '.'

  // check if byte available on the software serial port
  if(Serial1.available()){
    cmd = (Serial1.readStringUntil('.'));

    if (cmd.charAt(0) == 'c') { //click
      AbsoluteMouse.click();
      Serial.println("click");
    } 
    // Move to coordinate (16bit signed, -32768 - 32767)
    else if (cmd.charAt(0) == 'm') { //move
      String dx = cmd.substring(1,7);
      String dy = cmd.substring(8,13);
      float x = dx.toFloat();
      float y = dy.toFloat();
      AbsoluteMouse.moveTo(x,y);
      Serial.println("drag " + dx + "," + dy);
    } 
    else if (cmd.charAt(0) == 'd') { //drag
      AbsoluteMouse.press();
    }
    else if (cmd.charAt(0) == 'r') { //release
      AbsoluteMouse.release();
    }
    else {
      Serial.println("invalid");
      Serial.println(cmd);
    }
  }
}
