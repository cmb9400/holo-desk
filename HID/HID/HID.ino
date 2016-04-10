#include <Keyboard.h>
#include <Mouse.h>
#include <Math.h>

void setup() {
  // put your setup code here, to run once:
  Serial1.begin(9600);
}

void loop() {
    // Read serial input:
  while (Serial1.available() > 0) {
    String cmd = Serial1.readStringUntil('.');
    if (cmd.charAt(0) == 'c') {
      Mouse.click();
    } else if (cmd.charAt(0) == 'm') { 
      String dx = cmd.substring(1,7);
      String dy = cmd.substring(8,13);
      float x = dx.toFloat();
      float y = dy.toFloat();
      Mouse.move(x,y);
      Serial.println("drag " + dx + "," + dy);
    } else {
      Serial.println("invalid");
      Serial.println(cmd);
    }
  }
}

