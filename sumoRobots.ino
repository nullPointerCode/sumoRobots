#include "SumoRobot.h"


SumoRobot robot;
void setup() {
  Serial.begin(9600);//for debugging
}

void loop() {
  //comment/uncomment the following lines to test different robot speeds
  
  robot.go();
  delay(100);

  /*
  int* distanceSensorReadings = robot.getDistanceSensorReadings();
  bool* lineSensorReadings = robot.getLineSensorReadings();

  Serial.println("---  Line Sensors ---");
  String debugStr;
  
  debugStr = "Left Line Sensor: " + lineSensorReadings[0];
  Serial.println(debugStr);
  
  debugStr = "Right Line Sensor: " + lineSensorReadings[1];
  Serial.println(debugStr);
  
  debugStr = "Back Line Sensor: " + lineSensorReadings[2];
  Serial.println(debugStr);
  
  Serial.println("---  Distance Sensors ---");
  
  debugStr = String(distanceSensorReadings[0]);
  Serial.println(debugStr);
  
  debugStr = String(distanceSensorReadings[1]);
  Serial.println(debugStr);
  
  debugStr = String(distanceSensorReadings[2]);
  Serial.println(debugStr);

  delay(2000); //Sleep for 2 seconds
  */
}
