/**
Author: Randi Williams
Description: This code reads from the IMU to get roll, pitch, yaw, and position.
*/

#include "IMU.h"

IMU::IMU(wchar_t *portName) {
	IMU::ser = new Serial(portName);    // adjust as needed

	if (!ser->IsConnected())
		cout << "Sonar not connected" << endl;	
}

// Close serial connection
IMU::~IMU(void) {

}

float *IMU::getOrientation() {
	const int dataLength = 255;
	char *incomingData = (char *)malloc(sizeof(char) *256);			// don't forget to pre-allocate memory
	int readResult = 0;
	float *token;
	static float mag[3];

	readResult = ser->ReadData(incomingData, dataLength);
	incomingData[readResult] = 0;

	token = split(incomingData);
	for (int i = 0; i < 3; i++) {
		mag[i] = token[i] - baseline[i];
	}
	free(incomingData);
	return mag;
}

float* IMU::split(char *str) {
	static float elems[6];
	char * pch;
	int i = 0;

	str += 3;

	cout << "Splitting string: " << *str << endl;
	pch = strtok(str, " ");
	while (pch != NULL)
	{
		elems[i] = atof(pch);
		pch = strtok(NULL, " ");
		i++;
	}
	return elems;
}

void IMU::sort(float *array, int len) {
	float val;
	for (int i = 0; i < len; i++) {
		int j;
		val = array[i];
		for (j = i; j > 0 && array[j - 1]>val; j--) {
			array[j - 1] = array[j];
		}
		array[j] = val;
	}
}

/* This function reads the IMU for a few seconds to remove noise from the signal */
void IMU::baselineIMU() {
	const int SAMPLES = 5; // Number of samples to take for calculating baseline
	float magneto[3][SAMPLES];
	float acc[3][SAMPLES];

	cout << "Reading IMU baseline, leave sensor flat and still" << endl;

	for (int i = 0; i < SAMPLES; i++) {
		char *incomingData = (char *) malloc(sizeof(char)*256);			// don't forget to pre-allocate memory
		int dataLength = 255;
		int readResult = 0;
		stringstream ss;
		static string data(256, '0');
		float *token;
		bool flag = true;

		while (flag) {
			readResult = ser->ReadData(incomingData, dataLength);
			incomingData[readResult] = 0;
			if (incomingData[0] == '$' && incomingData[1] == '$' && incomingData[2] == '$') {
					cout << "We got one: " << incomingData << endl;
					flag = false;
			}
		}
		cout << "Char: " << incomingData << " " << readResult << endl;
		token = split(incomingData);

		magneto[0][i] = token[0];
		magneto[1][i] = token[1];
		magneto[2][i] = token[2];
		acc[0][i] = token[3];
		acc[0][i] = token[4];
		acc[0][i] = token[5];

		free(incomingData);
		waitKey(30);
	}
	cout << "Done taking readings" << endl;
	cout << "Sorting orientation data" << endl;
	sort(magneto[0], SAMPLES);
	sort(magneto[1], SAMPLES);
	sort(magneto[2], SAMPLES);
	cout << "Sorting accelerometer data" << endl;
	sort(acc[0], SAMPLES);
	sort(acc[1], SAMPLES);
	sort(acc[2], SAMPLES);

	IMU::baseline[0] = magneto[0][(int)SAMPLES / 2];
	IMU::baseline[1] = magneto[1][(int)SAMPLES / 2];
	IMU::baseline[2] = magneto[2][(int)SAMPLES / 2];
	IMU::baseline[3] = acc[0][(int)SAMPLES / 2];
	IMU::baseline[4] = acc[1][(int)SAMPLES / 2];
	IMU::baseline[5] = acc[2][(int)SAMPLES / 2];

	for (int i = 0; i < 6; i++) {
		cout << baseline[i] << ", ";
	}
	cout << endl;
}