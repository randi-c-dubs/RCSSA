#pragma once
#ifndef CAMERA_H
#define CAMERA_H

//Written by  Kyle Hounslow 2013

// modified by: Ahmad Kaifi, Hassan Althobaiti

// modified by Randi Williams

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include <sstream>
#include <string>
#include <iostream>
#include <vector>
#include <cv.h>
#include <highgui.h>
#include "Object.h"

using namespace std;
using namespace cv;

class Camera
{

public:
	//initial min and max HSV filter values.
	//these will be changed using trackbars
	int H_MIN = 0;
	int H_MAX = 256;
	int S_MIN = 0;
	int S_MAX = 256;
	int V_MIN = 0;
	int V_MAX = 256;
	//default capture width and height
	static const int FRAME_WIDTH = 640;
	static const int FRAME_HEIGHT = 480;
	//max number of objects to be detected in frame
	static const int MAX_NUM_OBJECTS = 10;
	//minimum and maximum object area
	static const int MIN_OBJECT_AREA = 50 * 50;
	static const int MAX_OBJECT_AREA = (int)(FRAME_HEIGHT*FRAME_WIDTH / 1.5);
	//names that will appear at the top of each window
	const string windowName = "Original Image";
	const string windowName1 = "HSV Image";
	const string windowName2 = "Thresholded Image";
	const string windowName3 = "After Morphological Operations";
	const string trackbarWindowName = "Trackbars";

	Camera();
	~Camera();

	void drawObject(vector<Object>, Mat&, Mat&, vector< vector<Point> >, vector<Vec4i>);
	void morphOps(Mat&);
	void trackFilteredObject(Object*, Mat, Mat, Mat&);
	void calibrateCamera();
	Object findObjectByColor(string);
	vector< Object > findAllObjects();
	vector< Object > seeObjects();
	void createTrackbars();

private:
	string intToString(int number);

	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;
	//video capture object to acquire webcam feed
	VideoCapture capture;

	//The following for canny edge detection
	Mat dst, detected_edges;
	Mat src, src_gray;
	int edgeThresh = 1;
	int lowThreshold;
	int const max_lowThreshold = 100;
	int ratio = 3;
	int kernel_size = 3;
};

#endif