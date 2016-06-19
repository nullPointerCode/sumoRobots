#ifndef SUMO_ROBOT_H
#define SUMO_ROBOT_H

#include "Arduino.h"
#include "Robot_1_Config.h" //Change this for a different robot

#define DISTANCE_SENSOR_THRESHOLD 200
#define NUM_READINGS 5
#define COUNTS_FOR_BACKING_UP 10
#define COUNTS_FOR_TURN 5
#define COUNTS_FOR_FULL_TURN 20

class SumoRobot{
	private:
		int m_distanceSensorReadings[3];
		bool m_lineSensorReadings[3];
		
		int m_actionBack = 0;
		int m_actionSharpRight = 0;
		int m_actionSharpLeft = 0;
		int m_actionForward = 0;
		
		String m_currAction = "";
		
		bool processActions();
		
		void go(int left, int right);

	public:
		SumoRobot();
		String go();
		bool* getLineSensorReadings();
		int* getDistanceSensorReadings();
};

#endif
