#include "SumoRobot.h"
#include <SoftwareSerial.h>

#define TX_PIN 10
#define RX_PIN 10

SumoRobot robot;
SoftwareSerial mySerial(TX_PIN, RX_PIN);
String prevText = "";

void setInitialRobotParameters(){
  mySerial.read();
}

void setup() {
  mySerial.begin(9600);//for debugging
  //mySerial.setTimeout(100);
}

void loop() {
  int command = mySerial.parseInt();
  robot.go(command);
  
  //String text = robot.go();
  //if (text != prevText){
	//  mySerial.println(text);
	//  prevText = text;
  //}
  delay(10);
}
