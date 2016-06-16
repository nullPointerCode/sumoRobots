#ifndef SUMO_ROBOT_H
#define SUMO_ROBOT_H

#include "Arduino.h"
#include "Robot_1_Config.h" //Change this for a different robot

#define DISTANCE_SENSOR_THRESHOLD 200
#define NUM_READINGS 5

class SumoRobot{
	private:
		int m_distanceSensorReadings[3];
		bool m_lineSensorReadings[3];
		
		int m_actionBack = 0;
		int m_actionSharpRight = 0;
		int m_actionSharpLeft = 0;
		int m_actionForward = 0;
		
		bool processActions();
		
		void go(int left, int right);

	public:
		SumoRobot();
		void go();
		bool* getLineSensorReadings();
		int* getDistanceSensorReadings();
};

#endif
