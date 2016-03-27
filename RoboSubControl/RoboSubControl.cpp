#include "RoboSubControl.h"

/**
* This constructor creates a RoboSubControl object and sets up all sensors and task operation.
* @mode is an integer [1] Full program, [2] Obstacle avoidance, [3] Calibration mode
*/
RoboSubControl::RoboSubControl(int mode) {
	// Setup tasks based on mode
	RoboSubControl::mode = mode;
}

RoboSubControl::~RoboSubControl() {}

void RoboSubControl::startRoboSubControl() {
	int taskNum = 0, taskTotal = 0;
	struct motorControl {
		// Stabilization motors
		int s_motor1; int s_motor2;
		// Movement motors
		int m_motor1; int m_motor2; int m_motor3; int m_motor4;
	} motorControl;

	switch (RoboSubControl::mode) {
	case RoboSubControl::CALIBRATION:
		calibrateCamera();
		break;

	case RoboSubControl::AVOID_OBJECTS:
		while (1) {
			while (true) {
				// Execute next step for task
				std::cout << "Looking for obstacles" << endl;
				std::cout << "Deciding where to move" << endl;
				std::cout << "Sending message to motors" << endl << endl;
			}
			std::cout << "Done tasks!" << endl;
		}

		break;

	case RoboSubControl::COMPETITION:
		while (1) {
			while (taskNum < taskTotal) {
				// Determine task
				std::cout << "Task " << taskNum << endl;

				// Execute next step for task
				std::cout << "Next step" << endl;

				// Update task
				std::cout << "Update Task" << endl;
				taskNum++;

				// Communicate with motors
				std::cout << "Sending signal to motors" << endl << endl;
			}
			std::cout << "Done tasks!" << endl;
		}

		break;

	default:
		std::cerr << "Houston, we have a problem! Wrong mode!" << endl;
		break;
	}
}

void calibrateCamera() {
	//Matrix to store each frame of the webcam feed
	Mat frontCameraFeed;// , downCameraFeed;
	Mat threshold;
	Mat HSV;

	//create slider bars HSV filtering
	createTrackbars();

	//video capture object to acquire webcam feed
	VideoCapture frontCapture; // downCapture;
	//open capture object at location zero (default location for webcam)
	frontCapture.open(0);
	//set height and width of capture frame
	frontCapture.set(CV_CAP_PROP_FRAME_WIDTH, FRAME_WIDTH);
	frontCapture.set(CV_CAP_PROP_FRAME_HEIGHT, FRAME_HEIGHT);
	//start an infinite loop where webcam feed is copied to cameraFeed matrix
	//all of our operations will be performed within this loop
	waitKey(1000);
	while (1){
		//store image to matrix
		frontCapture.read(frontCameraFeed);

		src = frontCameraFeed;

		if (!src.data) {
			break;
		}
		/* CAMERA CALIBRATION */
		//need to find the appropriate color range values
		// calibrationMode must be false

		//if in calibration mode, we track objects based on the HSV slider values.
		cvtColor(frontCameraFeed, HSV, COLOR_BGR2HSV);
		inRange(HSV, Scalar(H_MIN, S_MIN, V_MIN), Scalar(H_MAX, S_MAX, V_MAX), threshold);
		morphOps(threshold);
		imshow(windowName2, threshold);

		//the folowing for canny edge detec	  		
		/// Create a matrix of the same type and size as src (for dst)
		dst.create(src.size(), src.type());
		/// Convert the image to grayscale
		cvtColor(src, src_gray, CV_BGR2GRAY);
		/// Create a window
		namedWindow(window_name, CV_WINDOW_AUTOSIZE);
		/// Create a Trackbar for user to enter threshold
		// RANDI createTrackbar( "Min Threshold:", window_name, &lowThreshold, max_lowThreshold, CannyThreshold );
		/// Show the image
		// RANDI trackFilteredObject(threshold,HSV,cameraFeed);
	}
}