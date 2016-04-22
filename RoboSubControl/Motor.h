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

	void moveInDirection(int x, int y, int z, int r, int p, int w); // <x,y,z> <roll, pitch, yaw>

	void moveStraight(int speed);
	void moveBackwards(int speed);
	void moveUp(int speed);
	void moveDown(int speed);

	void turnLeft(int speed);
	void turnRight(int speed);

	void rollClockwise(int speed); // Left motors down, right motors up
	void rollCounterclockwise(int speed); // vice versa
	void rollForward(int speed); // front motors down, back motors up
	void rollBackward(int speed); // vice versa

private:
	int num_motors = 6;

};
#endif