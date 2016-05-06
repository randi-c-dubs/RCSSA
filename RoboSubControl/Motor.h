#pragma once
#ifndef MOTOR_H
#define MOTOR_H

#include <string>
#include <iostream>

using namespace std;

class Motor
{
public:
	Motor();
	~Motor(void);

	void moveInDirection(int, int, int, int, int, int); // <x,y,z> <roll, pitch, yaw>

	void moveStraight(int);
	void moveBackwards(int);
	void moveUp(int);
	void moveDown(int);

	void turnLeft(int);
	void turnRight(int);

	void rollClockwise(int); // Left motors down, right motors up
	void rollCounterclockwise(int); // vice versa
	void rollForward(int); // front motors down, back motors up
	void rollBackward(int); // vice versa

private:
	int num_motors = 6;

};
#endif