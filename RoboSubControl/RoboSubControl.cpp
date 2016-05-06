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

	Object o;
	vector< Object > objs;
	bool VISIBLE = true;

	switch (RoboSubControl::mode) {
	case RoboSubControl::CALIBRATION:
		front.calibrateCamera();
		break;

	case RoboSubControl::AVOID_OBJECTS:
		if (VISIBLE) {
			waitKey(1000);
			while (1) {
				objs = front.seeObjects();
				if (objs.empty())	// If there are no objects in the scene then move straight
					motor.moveStraight(255);
				else	// Otherwise turn
					motor.turnRight(255);
			}
		}
		else {
			while (1) {
				// See if there are any objects in the scene
				objs = front.findAllObjects();
				if (objs.empty())	// If there are no objects then move straight
					motor.moveStraight(255);
				else	// Otherwise turn
					motor.turnRight(255);
			}
		}
		break;

	case RoboSubControl::COMPETITION:
		std::cout << "Task: " << task << endl;
		while (task != RoboSubControl::Done) {
			// Within each task we 1) get the information we need, 2) send response to the motors 3) update the task
			switch (task) {
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