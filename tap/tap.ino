#include <Wire.h>
#include <ADXL345.h> 

#define THRESH_TAP 0x40                                                          //Tap threshold value
#define DUR 0x40                                                                 //Tap duration value
#define LATENT 0x40                                                              //Tap Latency value
#define WINDOW 0xFF                                                              //Tap window value
#define TAP_AXES 0x02                                                            //Axis control for single tap/double tap value 
#define POWER_CTL 0x09                                                           //Power-saving features control value
#define INT_ENABLE 0x60                                                          //Interrupt enable control value
#define INT_MAP 0x20                                                             //Interrupt mapping control value
#define DATA_FORMAT 0x0A                                                        //Data format control value

byte int_source;
boolean singleTap = false;
boolean doubleTap = false;

ADXL345 myACC = ADXL345(); //create an instance of class ADXL345() named myACC

void setup() {
  Serial.begin(9600);
  attachInterrupt(0, interrupt0, RISING);
  attachInterrupt(1, interrupt1, RISING);
  myACC.setTHRESH_TAP(THRESH_TAP);
  myACC.setDUR(DUR);
  myACC.setLATENT(LATENT);
  myACC.setWINDOW(WINDOW);
  myACC.setTAP_AXES(TAP_AXES);
  myACC.setPOWER_CTL(POWER_CTL);
  myACC.setINT_MAP(INT_MAP);
  myACC.setDATA_FORMAT(DATA_FORMAT);
  myACC.setINT_ENABLE(INT_ENABLE);
  Serial.println("swag is go");
}

void loop() {
  myACC.readINT_SOURCE(&int_source); //read int_source
  
  if(singleTap) {
    delay(500); //wait to see if another tap comes in
    if(!doubleTap) {
      Serial.println("Single Tap!");
    }
    else {                                                                       
      Serial.println("Double Tap");
    }
    singleTap = false;
    doubleTap = false;
  }
}

void interrupt0() {
  detachInterrupt(0);
  Serial.println("y");
  singleTap = true;
  attachInterrupt(0, interrupt0, RISING);
}

void interrupt1() {
  detachInterrupt(1);
  Serial.println("n");
  doubleTap = true;
  attachInterrupt(1, interrupt1, RISING);
}
