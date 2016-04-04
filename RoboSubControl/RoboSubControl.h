#pragma once
#ifndef ROBOSUBCONTROL_H
#define ROBOSUBCONTROL_H

#include <iostream>
#include <string>
#include <cv.h>
#include <highgui.h>
#include "ConfigNullException.h"
#include "Object.h"
#include "Camera.h"
#include "Task.h"

using namespace std;
using namespace cv;

class RoboSubControl
{

private:

	// member fields
	int mode;				// Mode submarine is in
	Camera front;

	// utility functions
public:
	static const int CALIBRATION = 1;
	static const int AVOID_OBJECTS = 2;
	static const int COMPETITION = 3;

	RoboSubControl(int);
	~RoboSubControl();	// RoboSub Destructor?
	void startRoboSubControl();
};

#endif