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
		front.calibrateCamera();
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