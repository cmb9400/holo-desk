/*
  Copyright (c) 2014-2015 NicoHood
  See the readme for credit to other people.
  AbsoluteMouse example
  Press a button to click, moveTo the mouse.
  You may also use SingleAbsoluteMouse to use a single report.
  See HID Project documentation for more infos
  https://github.com/NicoHood/HID/wiki/Mouse-API
  https://github.com/NicoHood/HID/wiki/AbsoluteMouse-API
*/

#include "HID-Project.h"

void setup() {

  // Sends a clean report to the host. This is important on any Arduino type.
  AbsoluteMouse.begin();
}

void loop() {
    // Move to coordinate (16bit signed, -32768 - 32767)
    // Moving to the same position twice will not work!
    // X and Y start in the upper left corner.
    AbsoluteMouse.moveTo(0, 0);
    delay(1000);
    AbsoluteMouse.moveTo(1024,1024);
    delay(1000);
    AbsoluteMouse.moveTo(5000,5000);
    delay(1000);


}
