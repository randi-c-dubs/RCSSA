#include "RoboSubControl.h"

/**
* This constructor creates a RoboSubControl object and sets up all sensors and task operation.
* @mode is an integer [3] Full program, [2] Obstacle avoidance, [1] Calibration mode
*/
RoboSubControl::RoboSubControl(int mode) {
	// Setup tasks based on mode
	RoboSubControl::mode = mode;
	RoboSubControl::front = new Camera();
	RoboSubControl::sonar = new Sonar(L"\\\\.\\COM8");
	RoboSubControl::imu = new IMU(L"\\\\.\\COM10");
}

RoboSubControl::~RoboSubControl() {}

void RoboSubControl::setMode(int mode){
	RoboSubControl::mode = mode;
}

void RoboSubControl::startRoboSubControl() {
	int taskNum = 0, taskTotal = 0;

	Object o;
	int dist, trigger, i;
	vector< Object > objs;
	bool VISIBLE = true;
	Vec4f slopeVec;
	task = RoboSubControl::OrangeAlignment;
	trigger = 0;

	switch (RoboSubControl::mode) {
	case RoboSubControl::CALIBRATION:
		front->calibrateCamera();
		//imu->baselineIMU();
		break;

	case RoboSubControl::AVOID_OBJECTS:
		if (VISIBLE) {
			waitKey(1000);
			while (1) {
				dist = sonar->getRangeReading();
				cout << "Distance = " << dist << endl;
				objs = front->seeObjects("CO Move Forward");
				if (!objs.empty()){
					o = objs.at(0);
					cout << "Full out:" << o.getVector() << endl;
				}

				if (objs.empty() && dist > 30){	// If there are no objects in the scene then move straight
					//front->writeText("Move Forward");
					motor->moveStraight(255);
				} else {	// Otherwise turn
					front->writeText("Turn Right");
					motor->turnRight(255);
				}
			}
		}
		else {
			while (1) {
				// See if there are any objects in the scene
				dist = sonar->getRangeReading();
				objs = front->findAllObjects("finding objects");
				if (objs.empty() && dist > 30)	// If there are no objects then move straight
					motor->moveStraight(255);
				else	// Otherwise turn
					motor->turnRight(255);
			}
		}
		break;

	case RoboSubControl::COMPETITION:
		std::cout << "Task: " << task << endl;

		while (task != RoboSubControl::Done) {
			// Within each task we 1) get the information we need, 2) send response to the motors 3) update the task
			switch (task) {

			case RoboSubControl::OrangeAlignment:
				//find the orange object (path) and align body to it.
				trigger = 0;
				while (trigger != 1){
					objs = front->seeObjects("Move Forward Searching for Orange");
					if (!objs.empty()){
						// there is an object identify if it is the orange path
						//cout << "GOTSOMETHING" << endl;
						for (i = 0; i < (objs.size()); i++){
							o = objs.at(i);
							cout << "FULL OUT:" << o.getVector() << endl;
							if (o.getType() == "orange"){
								//trigger = 1; // use trigger to hold the index of the objs

							}
						}

					}
				}
				objs = front->seeObjects("Stop Orange Located");
				o = objs.at(0);
				slopeVec = o.getVector();
				cout << "FULL OUT:" << slopeVec << endl;
				motor->stop();
				waitKey(1000);
				trigger = 0;


				//move to the the next task
				task = RoboSubControl::OrangeAlignment;
				front->seeObjects("Task Complete!");
				waitKey(3000);
				break;

			case RoboSubControl::AvoidYellowBuoy:
				//move straight towards objective until object is found.
				// find the yellow buoy and avoid it.
				while (trigger != 1){ // search for objects 
					motor->moveStraight(255);
					objs = front->seeObjects("Move Forward Searching for Yellow");
					if (!objs.empty()){
						// there is an object identify if it is the yellow buoy
						//cout << "GOTSOMETHING" << endl;
						for (i = 0; i < (objs.size()); i++){
							o = objs.at(i);
							if (o.getType() == "yellow"){
								trigger = 1;

							}
						}

					}
				}
				trigger = 0;
				while (trigger != 1){ // new loop to steer away from object until it is out of view
					motor->turnRight(255);
					objs = front->seeObjects("Turn Right Avoid Yellow Buoy");
					if (objs.empty()){ // no objects in view should be clear to move forward
						motor->moveStraight(255);
						trigger = 1;
					}
				}
				trigger = 0;
				task = RoboSubControl::AvoidGreenBuoy;
				front->seeObjects("Task Complete!");
				waitKey(3000);
				break;

			case RoboSubControl::HitRedBuoy:
				trigger = 0;
				// find the red buoy and avoid it.
				while (trigger != 1){ // search for objects 
					motor->moveStraight(255);
					objs = front->seeObjects("Move Forward Searching for Red");
					if (!objs.empty()){
						// there is an object identify if it is the green buoy
						//cout << "GOTSOMETHING" << endl;
						for (i = 0; i < (objs.size()); i++){
							o = objs.at(i);
							if (o.getType() == "red"){
								trigger = 1;

							}
						}

					}
				}

				trigger = 0;
				while (trigger != 1){
					motor->moveStraight(255);
					objs = front->seeObjects("Move Forward and Strike Red Buoy");
					/******/	if (!objs.empty() || (!objs.empty() && dist >30)){ // havent stuck it yet keep moving forward
						motor->moveStraight(255);
						front->seeObjects("Move Forward and Strike Red Buoy");
					}
					else{
						trigger = 1;
					}
				}

				task = RoboSubControl::AvoidGreenBuoy;
				front->seeObjects("Task Complete!");
				waitKey(3000);
				break;

			case RoboSubControl::AvoidGreenBuoy:
				//move straight towards objective until object is found.
				// find the green buoy and avoid it.
				while (trigger != 1){ // search for objects 
					motor->moveStraight(255);
					objs = front->seeObjects("Move Forward Searching for Green");
					if (!objs.empty()){
						// there is an object identify if it is the green buoy
						//cout << "GOTSOMETHING" << endl;
						for (i = 0; i < (objs.size()); i++){
							o = objs.at(i);
							if (o.getType() == "green"){
								trigger = 1;

							}
						}

					}
				}
				trigger = 0;
				while (trigger != 1){ // new loop to steer away from object until it is out of view
					motor->turnLeft(255);
					objs = front->seeObjects("Turn Left Avoid Green Buoy");
					if (objs.empty()){ // no objects in view should be clear to move forward
						motor->moveStraight(255);
						trigger = 1;
					}
				}
				trigger = 0;
				task = RoboSubControl::Done;
				front->seeObjects("Task Complete!");
				waitKey(3000);
				break;

			case RoboSubControl::MoveStraight:
				// Move forward Complementary filter?

				//motor.moveStraight(255);

				// After three feet
				break;
			default:
				std::cout << "Error: Invalid task number." << endl;
			}
			// Execute next step for task
			std::cout << "Next step" << endl;

			// Update task
			std::cout << "Update Task" << endl;

			// Communicate with motors
			std::cout << "Sending signal to motors" << endl << endl;
		}
		std::cout << "Done tasks!" << endl;

		break;

	default:
		std::cerr << "Houston, we have a problem! Wrong mode!" << endl;
		break;
	}
}