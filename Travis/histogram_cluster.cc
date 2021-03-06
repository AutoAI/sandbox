// histogram_cluster.cc

#include "histogram_cluster.h"

#include <unordered_map>
#include <vector>
#include <iostream>

#include "cluster_hash.h"

HistogramCluster::HistogramCluster(int x_resolution, int y_resolution, int block_dimension, int num_blocks_x, int num_blocks_y, int num_bins) {
	// copy parameters to class variables
	this -> x_resolution = x_resolution;
	this -> y_resolution = y_resolution;
	this -> block_dimension = block_dimension;
	this -> num_blocks_x = num_blocks_x;
	this -> num_blocks_y = num_blocks_y;
	this -> num_bins = num_bins;

	// calculate the offset from each block to the next
	int total_overlap_x = num_blocks_x * block_dimension - x_resolution;
	increment_x = block_dimension - total_overlap_x / (num_blocks_x - 1);
	int total_overlap_y = num_blocks_y * block_dimension - y_resolution;
	increment_y = block_dimension - total_overlap_y / (num_blocks_y - 1);

	// initialize the histogram buffer
	histograms = new uint16_t*[num_blocks_x * num_blocks_y];
	for(int y = 0; y < num_blocks_y; y++) {
		for(int x = 0; x < num_blocks_x; x++) {
			histograms[y * num_blocks_x + x] = new uint16_t[num_bins];
		}
	}

	// initialize the cluster map buffer
	cluster_map = new uint16_t[num_blocks_x * num_blocks_y];
	if(cluster_map == NULL) {
		std::cout << "Failed to allocate cluster_map <histogram_cluster.cc>" << std::endl;
	}
	for(int i = 0; i < num_blocks_x * num_blocks_y; i++) {
		cluster_map[i] = 65535; // uint16_t max value to indicate garbage
	}
}

// returns NULL on unsuccessful operation (we had too intense a block to see it properly)
uint16_t *HistogramCluster::doCluster(uint8_t *frame_buffer, float closeness_threshold, int blindness_threshold) {
	// CALCULATE HISTOGRAMS BLOCK-BY-BLOCK
	// (at some point I'd like to change this to pixel-by-pixel, mapping appropriate pixels directy to appropriate bins in block histograms)
	std::cout << "histograms" << std::endl;
	int max_y, max_x;
	uint16_t *current_histogram;
	// iterate over blocks
	for(int block_y = 0; block_y < num_blocks_y; block_y++) {
		for(int block_x = 0; block_x < num_blocks_x; block_x++) {
			current_histogram = histograms[block_y * num_blocks_x + block_x];
			// clear the histogram for this block
			for(int bin = 0; bin < num_bins; bin++) {
				current_histogram[bin] = 0;
			}
			// iterate over the pixels in the current block
			max_y = block_y * increment_y + block_dimension;
			for(int y = max_y - block_dimension; y < max_y; y++) {
				max_x = block_x * increment_x + block_dimension;
				for(int x = max_x - block_dimension; x < max_x; x++) {
					// increment the value of the appropriate bin
					current_histogram[frame_buffer[y * x_resolution + x] * num_bins / 256]++; // 256 is for maximum intensity of a pixel
				}
			}
			// check if our histogram had too many intensities in the top bin - the scene was too intense and we should probably do something about that
			if(current_histogram[num_bins - 1] >= blindness_threshold) {
				std::cout << "HistogramCluster::doCluster returned NULL (we're blind!)" << std::endl;
				return NULL;
			}
		}
	}

	std::cout << "clusters" << std::endl;
	// CALCULATE CLUSTERS
	uint16_t next_cluster_id = 0;
	bool left_same;
	bool top_same;
	int index_left;
	int index_top;
	int index;
	ClusterHash ch;
	uint16_t *current_block;
	// iterate over blocks
	for(int block_y = 0; block_y < num_blocks_y; block_y++) {
		std::cout << "y = " << block_y << ":\t";
		for(int block_x = 0; block_x < num_blocks_x; block_x++) {
			current_block = histograms[block_y * num_blocks_x + block_x];
			left_same = false;
			top_same = false;
			index = block_y * num_blocks_x + block_x;
			index_left = block_y * num_blocks_x + (block_x - 1);
			index_top = (block_y - 1) * num_blocks_x + block_x;
			// check to see if this block should be in the same cluster as the one to the left
			if(block_x != 0) {
				if(chiSquareDifference(current_block, histograms[index_left]) < closeness_threshold) {
					left_same = true;
				}
			}
			// check to see if this block should be in the same cluster as the one above
			if(block_y != 0) {
				if(chiSquareDifference(current_block, histograms[index_top]) < closeness_threshold) {
					top_same = true;
				}
			}
			// if they're both true, set this id equal to one of them and make those id's the same thing
			if(left_same && top_same) {
				// assign the id of this block to that of one of those
				cluster_map[block_y * num_blocks_x + block_x] = cluster_map[index_left];
				// first, make sure they don't have the same cluster id
				if(cluster_map[index_left] != cluster_map[index_top]) {
					// if not, state that these are equivalent cluster ids
					ch.addEquality(cluster_map[index_left], cluster_map[index_top]);
				}
			}
			// if only one is true, this block should be in the same cluster as that one
			else if(left_same) {
				cluster_map[index] = cluster_map[index_left];
			} else if(top_same) {
				cluster_map[index] = cluster_map[index_top];
			}
			// if neither is true, this block gets a new id
			else {
				cluster_map[block_y * num_blocks_x + block_x] = next_cluster_id++;
			}
			std::cout << cluster_map[block_y * num_blocks_x + block_x] << "\t";
		}
		std::cout << std::endl;
	}

	std::cout << "processing map" << std::endl;
	// process the map so that all ids in equivalence to one another map to one of those ids - commented out because its not quite ready yet
	std::unordered_map<int, int> cluster_equivalences = ch.genMap();

	std::cout << "mapping" << std::endl;
	// if values in our map are mapped to something, make them that thing instead so that clusters have 1 id only
	int size = num_blocks_y * num_blocks_x;
	for(int i = 0; i < size; i++) {
		if(cluster_equivalences.find(cluster_map[i]) != cluster_equivalences.end()) {
			cluster_map[i] = cluster_equivalences[cluster_map[i]];
		}
	}

	if(cluster_map == NULL) {
		std::cout << "HistogramCluster::doCluster returned NULL!" << std::endl;
	}

	return cluster_map;
}

// technically this returns 2 * the chi square difference, but we're just comparing them so it doesn't matter
float HistogramCluster::chiSquareDifference(uint16_t *histogram_P, uint16_t *histogram_Q) {
	float difference = 0;
	for(int bin = 0; bin < num_bins; bin++) {
		if(histogram_P[bin] + histogram_Q[bin] != 0) {
			difference += float(histogram_P[bin] - histogram_Q[bin]) * (histogram_P[bin] - histogram_Q[bin]) / float(histogram_P[bin] + histogram_Q[bin]);
		}
	}
	return difference;
}