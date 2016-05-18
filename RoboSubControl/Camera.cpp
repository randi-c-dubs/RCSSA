//Written by  Kyle Hounslow 2013

// modified by: Ahmad Kaifi, Hassan Althobaiti

// modified by Randi Williams, Kendall Queen

//Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software")
//, to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, 
//and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
//LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
//IN THE SOFTWARE.

#include "Camera.h"

Camera::Camera() {
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
}

Camera::~Camera() {}

void on_trackbar(int, void*)
{//This function gets called whenever a
	// trackbar position is changed

}

string Camera::intToString(int number){

	std::stringstream ss;
	ss << number;
	return ss.str();
}

void Camera::createTrackbars(){
	//create window for trackbars
	namedWindow(trackbarWindowName,0);
	//create memory to store trackbar name on window
	char TrackbarName[50];
	
	sprintf_s( TrackbarName, 50, "H_MIN %d", H_MIN);
	sprintf_s( TrackbarName, 50, "H_MAX %d", H_MAX);
	sprintf_s( TrackbarName, 50, "S_MIN %d", S_MIN);
	sprintf_s( TrackbarName, 50, "S_MAX %d", S_MAX);
	sprintf_s( TrackbarName, 50, "V_MIN %d", V_MIN);
	sprintf_s( TrackbarName, 50, "V_MAX %d", V_MAX);
	//create trackbars and insert them into window
	//3 parameters are: the address of the variable that is changing when the trackbar is moved(eg.H_LOW),
	//the max value the trackbar can move (eg. H_HIGH), 
	//and the function that is called whenever the trackbar is moved(eg. on_trackbar)
	//                                  ---->    ---->     ---->      
	createTrackbar( "H_MIN", trackbarWindowName, &H_MIN, H_MAX, on_trackbar );
	createTrackbar( "H_MAX", trackbarWindowName, &H_MAX, H_MAX, on_trackbar );
	createTrackbar( "S_MIN", trackbarWindowName, &S_MIN, S_MAX, on_trackbar );
	createTrackbar( "S_MAX", trackbarWindowName, &S_MAX, S_MAX, on_trackbar );
	createTrackbar( "V_MIN", trackbarWindowName, &V_MIN, V_MAX, on_trackbar );
	createTrackbar( "V_MAX", trackbarWindowName, &V_MAX, V_MAX, on_trackbar );
}

void Camera::drawObject(vector<Object> theObjects, Mat &frame, Mat &temp, vector< vector<Point> > contours, vector<Vec4i> hierarchy){
	cv::Point p1;
	cv::Point p2;
	for (int i = 0; i < theObjects.size(); i++){
		// y - y0 = m(x - x0)
		p1.x = theObjects.at(i).getVector()[2] - theObjects.at(i).getVector()[0] * FRAME_WIDTH; // 
		p1.y = theObjects.at(i).getVector()[3] - theObjects.at(i).getVector()[1] * FRAME_HEIGHT; // 
		p2.x = theObjects.at(i).getVector()[2] + theObjects.at(i).getVector()[0] * FRAME_WIDTH; // 
		p2.y = theObjects.at(i).getVector()[3] + theObjects.at(i).getVector()[1] * FRAME_HEIGHT; // 

		cv::drawContours(frame, contours, i, theObjects.at(i).getColor(), 3, 8, hierarchy);
		cv::rectangle(frame, theObjects.at(i).getBoundRect().tl(), theObjects.at(i).getBoundRect().br(), theObjects.at(i).getColor(), 2, 8, 0);
		cv::circle(frame, cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos()), 5, theObjects.at(i).getColor());
		cv::line(frame, p1, p2, theObjects.at(i).getColor(), 2, 8);
		cv::putText(frame, intToString(theObjects.at(i).getXPos()) + " , " + intToString(theObjects.at(i).getYPos()), cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos() + 20), 1, 1, theObjects.at(i).getColor());
		cv::putText(frame, theObjects.at(i).getType(), cv::Point(theObjects.at(i).getXPos(), theObjects.at(i).getYPos() - 20), 1, 2, theObjects.at(i).getColor());
	}
}

void Camera::morphOps(Mat &thresh){

	//create structuring element that will be used to "dilate" and "erode" image.
	//the element chosen here is a 3px by 3px rectangle
	Mat erodeElement = getStructuringElement( MORPH_RECT,Size(3,3));
	//dilate with larger element so make sure object is nicely visible
	Mat dilateElement = getStructuringElement( MORPH_RECT,Size(8,8));

	erode(thresh,thresh,erodeElement);
	erode(thresh,thresh,erodeElement);

	dilate(thresh,thresh,dilateElement);
	dilate(thresh,thresh,dilateElement);
}

void Camera::trackFilteredObject(Object* theObject, Mat threshold, Mat HSV, Mat &cameraFeed){

	vector <Object> objects;
	Mat temp;
	threshold.copyTo(temp);
	//these two vectors needed for output of findContours
	vector< vector<Point> > contours;
	vector<Vec4i> hierarchy;
	//find contours of filtered image using openCV findContours function
	findContours(temp,contours,hierarchy,CV_RETR_CCOMP,CV_CHAIN_APPROX_SIMPLE );

	/// Approximate contours to polygons + get bounding rects and circles
	vector<vector<Point> > contours_poly(contours.size());
	vector<Rect> boundRect(contours.size());
	vector<Vec4f> line(contours.size());
	for (int i = 0; i < contours.size(); i++)
	{
		approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
		boundRect[i] = boundingRect(Mat(contours_poly[i]));
		fitLine(Mat(contours_poly[i]), line[i], CV_DIST_L2, 0, 0.01, 0.01);
	}
	//use moments method to find our filtered object
	double refArea = 0;
	bool objectFound = false;
	if (hierarchy.size() > 0) {
		int numObjects = hierarchy.size();
		//if number of objects greater than MAX_NUM_OBJECTS we have a noisy filter
		if(numObjects<MAX_NUM_OBJECTS){
			for (int index = 0; index >= 0; index = hierarchy[index][0]) {

				Moments moment = moments((cv::Mat)contours[index]);
				double area = moment.m00;

		//if the area is less than 20 px by 20px then it is probably just noise
		//if the area is the same as the 3/2 of the image size, probably just a bad filter
		//we only want the object with the largest area so we safe a reference area each
				//iteration and compare it to the area in the next iteration.
				if(area>MIN_OBJECT_AREA){

					Object object;
					
					object.setXPos((int) (moment.m10/area));
					object.setYPos((int) (moment.m01/area));
					object.setVector(line[index]);
					object.setBoundRect(boundRect[index]);

					object.setType(theObject->getType());
					object.setColor(theObject->getColor());

					objects.push_back(object);

					objectFound = true;

				} else objectFound = false;
			}
			//let user know you found an object
			theObject->setFound(objectFound);
			if (objectFound == true) {
				//draw object location on screen
				drawObject(objects,cameraFeed,temp,contours,hierarchy);
			}
		} else putText(cameraFeed,"TOO MUCH NOISE! ADJUST FILTER",Point(0,50),1,2,Scalar(0,0,255),2);
	}
}

// function used to display text to original screen 
void Camera::writeText(string text){
	// makes text in the top left of camera feed image.
	putText(cameraFeed, text, Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);

}

Object Camera::findObjectByColor(string color, string text)  {
	capture.read(cameraFeed);
	src = cameraFeed;

	if (src.data) {

		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		Object obj(color);
		inRange(HSV, obj.getHSVmin(), obj.getHSVmax(), threshold);
		morphOps(threshold);
		trackFilteredObject(&obj, threshold, HSV, cameraFeed);
		if (obj.isFound()){
			putText(cameraFeed, text, Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
			return obj;
		}
	}
	return NULL;
}

vector< Object > Camera::findAllObjects(string text) {
	capture.read(cameraFeed);
	vector< Object > objects;
	src = cameraFeed;

	if (src.data) {
		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		//create some temp objects so that we can use their member functions/information
		Object orange("orange"), yellow("yellow"), red("red"), green("green");

		//first find orange objects
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		inRange(HSV, orange.getHSVmin(), orange.getHSVmax(), threshold);
		morphOps(threshold);
		trackFilteredObject(&orange, threshold, HSV, cameraFeed);
		//then yellows
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		inRange(HSV, yellow.getHSVmin(), yellow.getHSVmax(), threshold);
		morphOps(threshold);
		trackFilteredObject(&yellow, threshold, HSV, cameraFeed);
		//then reds
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		inRange(HSV, red.getHSVmin(), red.getHSVmax(), threshold);
		morphOps(threshold);
		trackFilteredObject(&red, threshold, HSV, cameraFeed);
		//then greens
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		inRange(HSV, green.getHSVmin(), green.getHSVmax(), threshold);
		morphOps(threshold);
		trackFilteredObject(&green, threshold, HSV, cameraFeed);

		// display text from robosub control
		putText(cameraFeed, text, Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
		if (orange.isFound()) objects.push_back(orange); // adjust parallel to rectangle
		else if (yellow.isFound()){
			// added text to show directions 
			//putText(cameraFeed, "Yellow Found Move Right", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
			objects.push_back(yellow);
		}
		else if (red.isFound()){
			// added text to show directions 
			//putText(cameraFeed, "RED Found CENTER ON OBJECT", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
			objects.push_back(red); // hit red
		}
		else if (green.isFound()){
			// added text to show directions 
			//putText(cameraFeed, "GREEN Found STRIKE OBJECT", Point(0, 50), 1, 2, Scalar(0, 0, 255), 2);
			objects.push_back(green); // hit green
		}

	}
	return objects;
}
vector< Object > Camera::seeObjects(string text) {
	//store image to matrix
	capture.read(cameraFeed);
	vector< Object > objects;
	src = cameraFeed;

	if (src.data) {

		objects = this->findAllObjects(text);;
		imshow(windowName, cameraFeed);
			
		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}
	return objects;
}

void Camera::calibrateCamera() {
	//create slider bars HSV filtering
	//createTrackbars();
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	int click = 0;
	waitKey(1000);
	while (click != NUMCOLORS){
		int SZ = 20;
		mousex = FRAME_WIDTH / 2;
		mousey = FRAME_HEIGHT / 2;
		//store image to matrix
		capture.read(cameraFeed);
		src = cameraFeed;
		//STARTING COLOR CALIBRATION
		//STEP1: Learning Phase
		//remove noise
		cv::GaussianBlur(src, denoised, cv::Size(5, 5), 2, 2);
		cv::cvtColor(denoised, hsv, CV_BGR2HSV);
		Point p1 = cv::Point(mousex - SZ, mousey - SZ);
		Point p2 = cv::Point(mousex + SZ, mousey + SZ);
		cv::rectangle(hsv, p1, p2, Scalar(255, 0, 0));

		//show camerafeed
		imshow(windowName, cameraFeed);

		//pick arbitrary 100x100px region in the center
		Rect region = cv::Rect(hsv.cols / 2 - SZ, hsv.rows / 2 - SZ, SZ * 2, SZ * 2);
		cv::Mat roi = hsv(region);

		//split channels to get hue only
	std:vector<cv::Mat> hsvPlanes, hsvVal;
		cv::split(roi, hsvPlanes);
		imshow("hsv", hsv);
		imshow("roi", roi);
		cv::split(hsv, hsvVal);
		//compute statsitics fo hue value
		cv::Scalar meanH, stddevH, meanS, stddevS, meanV, stddevV;
		cv::meanStdDev(hsvPlanes[0], meanH, stddevH);
		cv::meanStdDev(hsvPlanes[1], meanS, stddevS);
		cv::meanStdDev(hsvPlanes[2], meanV, stddevV);


		float minHue = meanH[0] - stddevH[0] * .2;
		float maxHue = meanH[0] + stddevH[0] * .2;
		float minSat = meanS[0] - stddevS[0] * 4;
		float maxSat = meanS[0] + stddevS[0] * 4;
		float minVal = meanV[0] - stddevV[0] * 4;
		float maxVal = meanV[0] + stddevV[0] * 4;

		//cout << "minHue" << minHue << "maxHue" << maxHue << "\n";
		//cout << "mean" << mean << "stddev" << stddev<<"\n";

		//STEP 2: detection phase
		cv::inRange(hsv, cv::Scalar(minHue, minSat, minVal), cv::Scalar(maxHue, maxSat, maxVal), imgThresh);
		imshow("thresholded", imgThresh);

		erode(imgThresh, processed, 5);  // minimizes noise
		dilate(processed, processed, 20);  // maximize left regions

		imshow("final", processed);

		if (!src.data) {
			break;
		}


		/* CAMERA CALIBRATION */
		//need to find the appropriate color range values


		//if in calibration mode, we track objects based on the HSV slider values.
		//convert image to HSV values
		/*
		cvtColor(cameraFeed, HSV, COLOR_BGR2HSV);
		inRange(HSV, Scalar(Camera::H_MIN, Camera::S_MIN, Camera::V_MIN), Scalar(Camera::H_MAX, Camera::S_MAX, Camera::V_MAX), threshold);
		morphOps(threshold);
		imshow(Camera::windowName2, threshold);
		*/




		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		//record hsv values with each click
		//each click represents a record of that color
		int key = waitKey(30);
		if (key != -1){
			string color = "";
			cout << "Enter Color: ";
			getline(cin, color);
			colors.push_back(color);
			minHSVvalues.push_back(Scalar(minHue));
			maxHSVvalues.push_back(Scalar(maxHue));
			click++;
		}
	}
}

//http://docs.opencv.org/master/d1/dee/tutorial_introduction_to_pca.html#gsc.tab=0

/*
int main(int argc, char* argv[])
{
	//if we would like to calibrate our filter values, set to true.
	bool calibrationMode = false;
	
	//Matrix to store each frame of the webcam feed
	Mat cameraFeed;
	Mat threshold;
	Mat HSV;

	if(calibrationMode){
		//create slider bars HSV filtering
		createTrackbars();
	}
	//video capture object to acquire webcam feed
	VideoCapture capture;
	//open capture object at location zero (default location for webcam)
	capture.open(0);
	//set height and width of capture frame
	capture.set(CV_CAP_PROP_FRAME_WIDTH,FRAME_WIDTH);
	capture.set(CV_CAP_PROP_FRAME_HEIGHT,FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	waitKey(1000);
	while(1){
		//store image to matrix
		capture.read(cameraFeed);

		src = cameraFeed;

  		if( !src.data )
  		{ return -1; }

		//convert frame from BGR to HSV colorspace
		cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);

		if(calibrationMode==true){

		//need to find the appropriate color range values
		// calibrationMode must be false

		//if in calibration mode, we track objects based on the HSV slider values.
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,Scalar(H_MIN,S_MIN,V_MIN),Scalar(H_MAX,S_MAX,V_MAX),threshold);
			morphOps(threshold);
			imshow(windowName2,threshold);
		
		//the folowing for canny edge detec	  		
			/// Create a matrix of the same type and size as src (for dst)
	  		dst.create( src.size(), src.type() );
	  		/// Convert the image to grayscale
	  		cvtColor( src, src_gray, CV_BGR2GRAY );
	  		/// Create a window
	  		namedWindow( window_name, CV_WINDOW_AUTOSIZE );
	  		/// Create a Trackbar for user to enter threshold
			// RANDI createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
	  		/// Show the image
			// RANDI trackFilteredObject(threshold,HSV,cameraFeed);
		}
		else{
			//create some temp fruit objects so that
			//we can use their member functions/information
			Object blue("orange"), yellow("yellow"), red("red"), green("green");

			//first find blue objects
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,blue.getHSVmin(),blue.getHSVmax(),threshold);
			morphOps(threshold);
			trackFilteredObject(blue,threshold,HSV,cameraFeed);
			//then yellows
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,yellow.getHSVmin(),yellow.getHSVmax(),threshold);
			morphOps(threshold);
			trackFilteredObject(yellow,threshold,HSV,cameraFeed);
			//then reds
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,red.getHSVmin(),red.getHSVmax(),threshold);
			morphOps(threshold);
			trackFilteredObject(red,threshold,HSV,cameraFeed);
			//then greens
			cvtColor(cameraFeed,HSV,COLOR_BGR2HSV);
			inRange(HSV,green.getHSVmin(),green.getHSVmax(),threshold);
			morphOps(threshold);
			trackFilteredObject(green,threshold,HSV,cameraFeed);

		}
		//show frames 
		//imshow(windowName2,threshold);

		imshow(windowName,cameraFeed);
		//imshow(windowName1,HSV);

		//delay 30ms so that screen can refresh.
		//image will not appear without this waitKey() command
		waitKey(30);
	}
	return 0;
}
*/