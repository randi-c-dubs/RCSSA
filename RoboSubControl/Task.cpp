#include "Task.h"

Task::Task()
{
	//set values for default constructor
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
