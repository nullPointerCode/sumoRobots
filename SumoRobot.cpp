#include "SumoRobot.h"

SumoRobot::SumoRobot(){
	//Set up the motor pins as output pins
	pinMode(LeftMotorDirectionA, OUTPUT);
	pinMode(LeftMotorDirectionB, OUTPUT);
	pinMode(LeftMotorSpeed, OUTPUT);
	pinMode(RightMotorDirectionA, OUTPUT);
	pinMode(RightMotorDirectionB, OUTPUT);
	pinMode(RightMotorSpeed, OUTPUT);
	
	//Set up line and distance sensors as inputs
	pinMode(LeftLineSensor, INPUT);
	pinMode(RightLineSensor, INPUT);
	pinMode(BackLineSensor, INPUT);
	pinMode(LeftDistanceSensor, INPUT);
	pinMode(RightDistanceSensor, INPUT);
	pinMode(BackDistanceSensor, INPUT);
}


int* SumoRobot::getLineSensorReadings(){
	m_lineSensorReadings[0] = digitalRead(LeftLineSensor);//Left Line Sensor
	m_lineSensorReadings[1] = digitalRead(RightLineSensor);//Right Line Sensor
	m_lineSensorReadings[2] = digitalRead(BackLineSensor);//Back Line Sensor
	return m_lineSensorReadings;
}

int* SumoRobot::getDistanceSensorReadings(){
	m_distanceSensorReadings[0] = analogRead(LeftDistanceSensor);//Left Distance Sensor
	m_distanceSensorReadings[1] = analogRead(RightDistanceSensor);//Right Distance Sensor
	m_distanceSensorReadings[2] = analogRead(BackDistanceSensor);//Back Distance Sensor
	return m_distanceSensorReadings;
}


//Controls the speeds of the left and right motor
//The speed should be between -255 (reverse) and +255 (forward)
//A speed of 0 indicates a stop
void SumoRobot::go(int left, int right){
	if (left > 0){//Left Motor Forward
		if (left > 255){
			left = 255;
		}
		analogWrite(LeftMotorSpeed, left);
		digitalWrite(LeftMotorDirectionA, HIGH);
		digitalWrite(LeftMotorDirectionB, LOW);
	} else if (left < 0){//Left Motor Reverse
		if (left < 255){
			left = -255;
		}
		left = left * -1;
		analogWrite(LeftMotorSpeed, left);
		digitalWrite(LeftMotorDirectionB, HIGH);
		digitalWrite(LeftMotorDirectionA, LOW);
	} else {
		//Stop
		analogWrite(LeftMotorSpeed, 0);
	}
	
	if (right > 0){//Right Motor Forward
		if (right > 255){
			right = 255;
		}
		analogWrite(RightMotorSpeed, right);
		digitalWrite(RightMotorDirectionB, HIGH);
		digitalWrite(RightMotorDirectionA, LOW);
	} else if (right < 0){//Right Motor Reverse
		if (right < 255){
			right = -255;
		}
		right = right * -1;
		analogWrite(RightMotorSpeed, right);
		digitalWrite(RightMotorDirectionA, HIGH);
		digitalWrite(RightMotorDirectionB, LOW);
	} else {
		//Stop
		analogWrite(RightMotorSpeed, 0);
	}
}