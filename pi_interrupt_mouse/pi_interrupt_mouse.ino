#include "HID-Project.h"
//  See HID Project documentation for more infos
//  https://github.com/NicoHood/HID/wiki/Mouse-API
//  https://github.com/NicoHood/HID/wiki/AbsoluteMouse-API


enum states { // fuck you wade
  NO_CLICK, // No tap has been detected yet
  CLICK, // A single tap has been detected
  HOLD, // A double tap has been detected
  DRAG, // No tap has been detected since Hold was sent
  RELEASE, // A tap has been detected during the Drag state
};

enum states state = NO_CLICK; // Set the init state too no click

void setup() {
  Serial.begin(9600);    // serial / USB port
  Serial1.begin(9600);   // Arduino RX - white, Arduino TX - green
  AbsoluteMouse.begin();
  attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING); //interrupt pin from Mega (hock up pin 35 mega to 2 leo)
  
  delay(2000);
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
  if(Serial1.available()){
    cmd = (Serial1.readStringUntil('.'));
    //Serial.println(cmd);
    String dx = cmd.substring(0,4);
    String dy = cmd.substring(5,9);
    float x = dx.toFloat();
    float y = dy.toFloat();
    AbsoluteMouse.moveTo(x,y);
  } 
  
}

void interrupt() {
  //detachInterrupt(digitalPinToInterrupt(2));
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
  //attachInterrupt(digitalPinToInterrupt(2), interrupt, RISING);
}
