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

	int x, y;
	Object o;

	switch (RoboSubControl::mode) {
	case RoboSubControl::CALIBRATION:
		front.calibrateCamera();
		break;

	case RoboSubControl::AVOID_OBJECTS:
		while (1) {
			while (1) {
				// Execute next step for task
				/*o = front.findObjectByColor("blue");
				if (o.getXPos() > 0 && o.getYPos() > 0)
					cout << o.getXPos() << " " << o.getYPos() << endl;
				else
					cout << "No object" << endl;
					*/
				std::cout << "Looking for obstacles" << endl;
				std::cout << "Deciding where to move" << endl;
				std::cout << "Sending message to motors" << endl << endl;
			}
			std::cout << "Done tasks!" << endl;
		}

		break;

	case RoboSubControl::COMPETITION:
		std::cout << "Task: " << RoboSubControl::task << endl;
		while (RoboSubControl::task != RoboSubControl::Done) {
			// Within each task we 1) get the information we need, 2) send response to the motors 3) update the task
			switch (RoboSubControl::task) {
			case RoboSubControl::MoveStraight:
				// Move forward
				motor.moveStraight(255);

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