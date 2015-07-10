#pragma once

class HistogramCluster{
public:
	HistogramCluster(int x_resolution, int y_resolution, int block_dimension, int num_blocks_x, int num_blocks_y, int num_bins);
	
	uint16_t *doCluster(uint8_t *frame_buffer, int closeness_threshold, int blindness_threshold);
	int chiSquareDifference(uint16_t *histogram_P, uint16_t *histogram_Q);
}