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

void moveInDirection(int x, int y, int z, int r, int p, int w) { // <x,y,z> <roll, pitch, yaw>
}

void moveStraight(int speed) {}
void moveBackwards(int speed) {}
void moveUp(int speed) {}
void moveDown(int speed) {}

void turnLeft(int speed) {}
void turnRight(int speed) {}

void rollClockwise(int speed) { // Left motors down, right motors up
}
void rollCounterclockwise(int speed) { // vice versa
}
void rollForward(int speed) { // front motors down, back motors up
}
void rollBackward(int speed) { // vice versa
}
