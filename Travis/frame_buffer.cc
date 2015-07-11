// frame_buffer.cc

#include "frame_buffer.h"

#include <iostream>

FrameBuffer::FrameBuffer(std::string file_name, int x_resolution, int y_resolution){
	// dynamically allocate the frame buffer
	int num_pixels = x_resolution * y_resolution;
	int buffer_size = sizeof(uint16_t) * num_pixels;
	frame_buffer = new uint16_t[buffer_size];
	if(!frame_buffer) {
		std::cout << "Failed to allocate buffer" << std::endl;
		return 1;
	}

	// open the file
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
	num_frames = file_size / buffer_size;

	frame_number = 0;
}

uint16_t *FrameBuffer::next(){
	// load each frame into the buffer
	is.read(reinterpret_cast<char*>(frame_buffer), buffer_size);

	// reverse order of bytes
	for(int i = 0; i < num_pixels; i++) {
		frame_buffer[i] = (frame_buffer[i] >> 8) | ((frame_buffer[i] & 0x00FF) << 8);
	}

	frame_number++;

	return frame_buffer;
}

bool FrameBuffer::hasNext(){
	return frame_number == num_frames;
}

int FrameBuffer::numFrames(){
	return num_frames;
}