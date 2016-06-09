#ifndef MOTOR_CONTROL_H
#define MOTOR_CONTROL_H

#include "Arduino.h"
#include "Robot_1_Config.h" //Change this for a different robot

class SumoRobot{
	private:
		int m_distanceSensorReadings[3];
		int m_lineSensorReadings[3];

	public:
		SumoRobot();
		void go(int left, int right);
		int* getLineSensorReadings();
		int* getDistanceSensorReadings();
};

#endif
