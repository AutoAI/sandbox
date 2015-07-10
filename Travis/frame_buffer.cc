// frame-buffer.cc

#include <stdint.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <ctime>

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

	// dynamically allocate the frame buffer
	int num_pixels = x_resolution * y_resolution;
	int buffer_size = sizeof(uint16_t) * num_pixels;
	uint16_t *frame_buffer = new uint16_t[buffer_size];
	if(!frame_buffer) {
		std::cout << "Failed to allocate buffer" << std::endl;
		return 1;
	}

	// open the file
	std::ifstream is;
    is.open(file_name.c_str(), is.binary);
    if(!is.is_open()) {
    	std::cout << "Failed to open the file." << std::endl;
    	return 1;
    }

    // find the number of frames
	is.seekg(0, is.end);
	int file_size = is.tellg();
	is.seekg (0, is.beg);
	if(file_size == 0) {
		std::cout << "Warning: file size is 0" << std::endl;
	}
	int num_frames = file_size / buffer_size;

	// load each frame into the buffer
	for(int frame_number = 0; frame_number < num_frames; frame_number++) {
		is.read(reinterpret_cast<char*>(frame_buffer), buffer_size);
		// reverse order of bytes
		for(int i = 0; i < num_pixels; i++) {
			frame_buffer[i] = ((frame_buffer[i] & 0xFF00) >> 8) + ((frame_buffer[i] & 0x00FF) << 8);
		}
		std::cout << "First element of frame: " << frame_buffer[0] << std::endl;
	}

	// deallocate the frame buffer
	delete[] frame_buffer;

	// print the program's run duration
	double program_duration = (std::clock() - program_start_time) / (double) CLOCKS_PER_SEC;
	std::cout << "Loaded " << num_frames << " frames in " << program_duration << " seconds (" << num_frames / program_duration << " frames per second)." << std::endl;

	return 0;
}