/** 
	Author: Randi Williams
	Description: This code implements Task objects and defines the parameters for running types of tasks.
*/

#include "Task.h"

Task::Task()
{
	//Set values for default constructor
	setType("Task");

}

Task::Task(string name){

	setType(name);

}

Task::~Task(void)
{
}

void Task::setType(string name){

	Task::type = name;
}

string Task::getType() {
	return Task::type;
}

void Task::execute() {
	cout << "Doing task " << Task::getType() << endl;
}

bool Task::isComplete() {
	return true;
}
