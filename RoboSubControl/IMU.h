#pragma once
#ifndef IMU_H
#define IMU_H

#include <cv.h>
#include <highgui.h>
#include <string>
#include <chrono>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "SerialClass.h"

using namespace std;
using namespace cv;

class IMU
{
public:
	IMU(wchar_t *portName);
	~IMU(void);

	float* IMU::getOrientation();
	void IMU::baselineIMU();

private:
	Serial *ser;
	void IMU::sort(float *, int);
	float *IMU::split(char *);
	float *baseline;

};
#endif