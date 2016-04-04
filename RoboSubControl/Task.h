#pragma once
#ifndef TASK_H
#define TASK_H

#include <string>
#include <iostream>

using namespace std;

class Task
{
public:
	Task();
	~Task(void);

	Task(string name);

	void execute();
	bool isComplete();

	void setType(string name);
	string getType();

private:

	string type;
};
#endif 