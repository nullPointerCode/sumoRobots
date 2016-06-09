#include "SumoRobot.h"


SumoRobot robot;
void setup() {
  Serial.begin(9600);//for debugging
}

void loop() {
  //comment/uncomment the following lines to test different robot speeds
  
  robot.go(128, 128);//go forward at medium speed  
  //robot.go(-128, 128);//go back at medium speed
  //robot.go(0, 0);//stop the robot
  //robot.go(0, 128);//turn left
  //robot.go(128, 0);//turn right

  int* distanceSensorReadings = robot.getDistanceSensorReadings();
  int* lineSensorReadings = robot.getLineSensorReadings();

  Serial.println("---  Line Sensors ---");
  String debugStr;
  
  debugStr = "Left Line Sensor: " + lineSensorReadings[0];
  Serial.println(debugStr);
  
  debugStr = "Right Line Sensor: " + lineSensorReadings[1];
  Serial.println(debugStr);
  
  debugStr = "Back Line Sensor: " + lineSensorReadings[2];
  Serial.println(debugStr);
  
  Serial.println("---  Distance Sensors ---");
  
  debugStr = "Left Distance Sensor: " + distanceSensorReadings[0];
  Serial.println(debugStr);
  
  debugStr = "Right Distance Sensor: " + distanceSensorReadings[1];
  Serial.println(debugStr);
  
  debugStr = "Back Distance Sensor: " + distanceSensorReadings[2];
  Serial.println(debugStr);

  delay(2000); //Sleep for 2 seconds
}
