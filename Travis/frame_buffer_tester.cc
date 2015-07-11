// frame_buffer_tester.cc

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

#include "frame_buffer.h"

int main(int argc, char *argv[]) {
	// make sure the input is valid
	if(argc != 4) {
		std::cout << "Invalid number of arguments: arguments are file_name, x_resolution, and y_resolution" << std::endl;
		return 1;
	}

	if(std::atoi(argv[2]) < 1 || std::atoi(argv[3]) < 1) {
		std::cout << "Resolution arguments must be positive integers." << std::endl;
		return 1;
	}

	// start timing the program
	std::clock_t program_start_time = std::clock();

	// read in arguments
	std::string file_name = argv[1];
	int x_resolution = std::atoi(argv[2]);
	int y_resolution = std::atoi(argv[3]);

	FrameBuffer fb(file_name, x_resolution, y_resolution);

	while(fb.hasNext()) {
		fb.next();
	}

	// print the program's run duration
	double program_duration = (std::clock() - program_start_time) / (double) CLOCKS_PER_SEC;
	std::cout << "Loaded " << fb.numFrames() << " frames in " << program_duration << " seconds (" << fb.numFrames() / program_duration << " frames per second)." << std::endl;

	return 0;
}