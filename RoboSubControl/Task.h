#pragma once
#include <string>

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

private:

	string type;
};
