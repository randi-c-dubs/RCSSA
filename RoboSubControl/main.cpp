#include <iostream>
#include "RoboSubControl.h"

int main(int argc, char* argv[]) {
	// Ensure that the serial com and robosub mode were passed as command-line arguments
	if (argc != 2) {
		std::cerr << "Missing the mode." 
			<< "\nTo connect to an address use: RoboSub.exe [Mode]" << std::endl;
		return 1;
	}
	
	// Begin the actual autonomous control program
	try {
		RoboSubControl sub(stoi(argv[1]));
		sub.startRoboSubControl();
	}
	catch (ConfigNullException &e) {
		//There was an error
		std::cerr << e.what() << std::endl;
	}

	return 0;
}