/**
Author: Randi Williams
Description: This code reads  from the sonar for the robosub.
*/

#include "Sonar.h"

// Open serial connection
Sonar::Sonar(wchar_t *portName) {
	Sonar::ser = new Serial(portName);    // adjust as needed

	if (!ser->IsConnected())
		cout << "Sonar not connected" << endl;
}

// Close serial connection
Sonar::~Sonar(void) {

}

int Sonar::getRangeReading() {
	char incomingData[256] = "";			// don't forget to pre-allocate memory
	int dataLength = 255;
	int readResult = 0;
	int length = 0;

	readResult = ser->ReadData(incomingData, dataLength);
	incomingData[readResult] = 0;
	if ((length = atoi(incomingData)) == 0) {
		length = INT16_MAX;
	}

	return length;
}