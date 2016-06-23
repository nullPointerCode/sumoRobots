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
	
	int left = 0;//Left Line Sensor
	int right = 0;//Right Line Sensor
	int back = 0;//Back Line Sensor
	
	//Average readings
	for (int i = 0; i < NUM_READINGS; i++){
		left += analogRead(LeftLineSensor);
		right += analogRead(RightLineSensor);
		back += analogRead(BackLineSensor);
	}
	
	if (left <= NUM_READINGS * LINE_SENSOR_THRESHOLD){
		m_lineSensorReadings[0] = true;
	} else {
		m_lineSensorReadings[0] = false;
	}
	
	if (right <= NUM_READINGS * LINE_SENSOR_THRESHOLD){
		m_lineSensorReadings[1] = true;
	} else {
		m_lineSensorReadings[1] = false;
	}
	
	if (m_backLineSensorEnabled && back <= NUM_READINGS * LINE_SENSOR_THRESHOLD){
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
	
	if (m_backDistanceSensorEnabled){
		m_distanceSensorReadings[2] = m_distanceSensorReadings[2] / NUM_READINGS;
	} else {
		m_distanceSensorReadings[2] = 0;
	}
	
	return m_distanceSensorReadings;
}


//Controls the speeds of the left and right motor
//The speed should be between -255 (reverse) and +255 (forward)
//A speed of 0 indicates a stop
void SumoRobot::go(int left, int right){
	
	//scale down the speeds
	left = (left * m_maxSpeed)/255;
	right = (right * m_maxSpeed)/255;
	
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

void SumoRobot::go(int command){
  int x = command / 100;
  int y = command - x * 100;

  x = ((x - 50) * 255) / 50;
  y = ((y - 50) * 255) / 50;
  go(x, y);
}

bool SumoRobot::processActions(){
	if (m_actionBack > 0){
		m_actionBack--;
		go(-255, -255);
		m_currAction = "Going back";
		return true;
	}
	
	if (m_actionSharpRight > 0){
		m_actionSharpRight--;
		go(255, -255);
		m_currAction = "Sharp Right";
		return true;
	}
	
	if (m_actionSharpLeft > 0){
		m_actionSharpLeft--;
		go(-255, 255);
		m_currAction = "Sharp Left";
		return true;
	}
	
	if (m_actionForward > 0){
		m_actionForward--;
		go(255, 255);
		m_currAction = "Going forward";
		return true;
	}
	
	return false;
}

String SumoRobot::go(){
	
	if (!processActions()){	
		getLineSensorReadings();
		
		//check if robot is on the white ring
		if (m_lineSensorReadings[0] && m_lineSensorReadings[1]){
			m_actionBack = COUNTS_FOR_BACKING_UP; //go back for 10 counts
			m_actionSharpLeft = COUNTS_FOR_TURN;  //turn sharp left for 5 counts
			return "Both line sensors are active, will back up and turn";
		} else if (m_lineSensorReadings[0]){
			m_actionBack = COUNTS_FOR_BACKING_UP/2;
			m_actionSharpLeft = COUNTS_FOR_TURN;
			return "Left line sensor is active, will back up and turn";
		} else if (m_lineSensorReadings[1]){
			m_actionBack = COUNTS_FOR_BACKING_UP/2;
			m_actionSharpRight = COUNTS_FOR_TURN;
			return "Right line sensor is active, will back up and turn";
		} else if (m_lineSensorReadings[2]){
			//we are being pushed back, push forward  and then turn
			m_actionForward = COUNTS_FOR_BACKING_UP;
			m_actionSharpRight = COUNTS_FOR_TURN;
			return "Back line sensor is active, will go forward and turn";
		}
		
		//check if we see the other robot
		getDistanceSensorReadings();
		
		if (m_distanceSensorReadings[0] >= DISTANCE_SENSOR_THRESHOLD && 
			m_distanceSensorReadings[1] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot in front of us, attack!
			go(255, 255);
			return "Both distance sensors detect the other robot, attack full speed forward";
		} else if (m_distanceSensorReadings[0] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot on our left, sharp turn left
			go(-255, 255);
			return "Left distance sensors detects the other robot, sharp left";
		} else if (m_distanceSensorReadings[1] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot on our right, sharp turn right
			go(255, -255);
			return "Right distance sensors detects the other robot, sharp right";
		} else if (m_distanceSensorReadings[2] >= DISTANCE_SENSOR_THRESHOLD){
			//we see the other robot behind us, turn around
			m_actionSharpLeft = COUNTS_FOR_FULL_TURN;
			return "Back distance sensors detects the other robot, turn around";
		}
		
		//let's look for the other robot
		go(100, 125);
		return "Find the other robot";
	} else {
		return "Executing previously issued action: " + m_currAction;
	}
}
