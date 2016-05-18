/**
Author: Randi Williams
Description: This code implements Motor control for the six motors on the robosub.
*/

#include "Motor.h"

Motor::Motor()
{
	// Make connections to all six motors and save them
	// Object m1, m2, m3, m4, m5, m6;
}

Motor::~Motor(void)
{
	// Close all connections to motors
}

void Motor::moveInDirection(int x, int y, int z, int r, int p, int w) { // <x,y,z> <roll, pitch, yaw>
}

void Motor::stop() {
	cout << "Stopping" << endl;
}

void Motor::moveStraight(int speed) {
	cout << "Going straigt" << endl;
	//m2.ser.write(1400);

}
void Motor::moveBackwards(int speed) {}
void Motor::moveUp(int speed) {}
void Motor::moveDown(int speed) {}

void Motor::turnLeft(int speed) {}
void Motor::turnRight(int speed) {
	cout << "Turn right" << endl;
}

void Motor::rollClockwise(int speed) { // Left motors down, right motors up
}
void Motor::rollCounterclockwise(int speed) { // vice versa
}
void Motor::rollForward(int speed) { // front motors down, back motors up
}
void Motor::rollBackward(int speed) { // vice versa
}
