#pragma once
#ifndef SONAR_H
#define SONAR_H

#include <string>
#include <iostream>
#include <thread>         // std::this_thread::sleep_for
#include <chrono>         // std::chrono::seconds
#include "SerialClass.h"

using namespace std;

class Sonar
{
public:
	Sonar(wchar_t *portName);
	~Sonar(void);

	int getRangeReading();

private:
	Serial *ser;

};
#endif