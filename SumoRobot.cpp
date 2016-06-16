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


bool* SumoRobot::getLineSensorReadings(){
	int left = 0;
	int right = 0;
	int back = 0;
	
	//Average readings
	for (int i = 0; i < NUM_READINGS; i++){
		if (digitalRead[LeftDistanceSensor]){
			left += 1;
		}
		if (digitalRead[RightLineSensor]){
			right += 1;
		}
		if (digitalRead[BackLineSensor]){
			back += 1;
		}
	}
	if (left >= NUM_READINGS/2){
		m_lineSensorReadings[0] = true;
	} else {
		m_lineSensorReadings[0] = false;
	}
	
	if (right >= NUM_READINGS/2){
		m_lineSensorReadings[1] = true;
	} else {
		m_lineSensorReadings[1] = false;
	}
	
	if (back >= NUM_READINGS/2){
		m_lineSensorReadings[2] = true;
	} else {
		m_lineSensorReadings[2] = false;
	}
	
	return m_lineSensorReadings;
}

int* SumoRobot::getDistanceSensorReadings(){
	
	m_distanceSensorReadings[0] = 0;//Left Distance Sensor
	m_distanceSensorReadings[1] = 0;//Right Distance Sensor
	m_distanceSensorReadings[2] = 0;//Back Distance Sensor
	
	//Average readings
	for (int i = 0; i < NUM_READINGS; i++){
		m_distanceSensorReadings[0] += analogRead(LeftDistanceSensor);
		m_distanceSensorReadings[1] += analogRead(RightDistanceSensor);
		m_distanceSensorReadings[2] += analogRead(BackDistanceSensor);
	}
	
	m_distanceSensorReadings[0] = m_distanceSensorReadings[0] / NUM_READINGS;
	m_distanceSensorReadings[1] = m_distanceSensorReadings[1] / NUM_READINGS;
	m_distanceSensorReadings[2] = m_distanceSensorReadings[2] / NUM_READINGS;
	
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

bool SumoRobot::processActions(){
	if (m_actionBack > 0){
		m_actionBack--;
		go(-255, -255);
		return true;
	}
	
	if (m_actionSharpRight > 0){
		m_actionSharpRight--;
		go(255, -255);
		return true;
	}
	
	if (m_actionSharpLeft > 0){
		m_actionSharpLeft--;
		go(-255, 255);
		return true;
	}
	
	if (m_actionForward > 0){
		m_actionForward--;
		go(255, 255);
		return true;
	}
	
	return false;
}

void SumoRobot::go(){
	
	if (!processActions()){	
		getLineSensorReadings();		
		
		//check if robot is on the white ring
		if (m_lineSensorReadings[0] && m_lineSensorReadings[1]){
			m_actionBack = COUNTS_FOR_BACKING_UP; //go back for 10 counts
			m_actionSharpLeft = COUNTS_FOR_TURN;  //turn sharp left for 5 counts
			return;
		} else if (m_lineSensorReadings[0]){
			m_actionBack = COUNTS_FOR_BACKING_UP/2;
			m_actionSharpLeft = COUNTS_FOR_TURN;
			return;
		} else if (m_lineSensorReadings[1]){
			m_actionBack = COUNTS_FOR_BACKING_UP/2;
			m_actionSharpRight = COUNTS_FOR_TURN;
			return;
		} else if (m_lineSensorReadings[2]){
			//we are being pushed back, push forward  and then turn
			m_actionForward = COUNTS_FOR_BACKING_UP;
			m_actionSharpRight = COUNTS_FOR_TURN;
		}
		
		//check if we see the other robot
		getDistanceSensorReadings();
		
		if (m_distanceSensorReadings[0] >= DISTANCE_SENSOR_THRESHOLD && 
			m_distanceSensorReadings[1] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot in front of us, attack!
			go(255, 255);
		} else if (m_distanceSensorReadings[0] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot on our left, sharp turn left
			go(-255, 255);
		} else if (m_distanceSensorReadings[1] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot on our right, sharp turn right
			go(255, -255);
		} else if (m_distanceSensorReadings[2] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot behind us, turn around
			m_actionSharpLeft = COUNTS_FOR_FULL_TURN;
		}
		
		//let's look for the other robot
		go(100, 125);
	}
}