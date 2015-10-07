/*
 *  GaussianWhiteNoiseMask.cpp
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by bkennedy on 11/13/08.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "GaussianMask.h"

GaussianMask::GaussianMask(const shared_ptr<Variable> &_size,
							 const shared_ptr<Variable> &_mean,
							 const shared_ptr<Variable> &_std_dev) : Mask(_size) {
	mean = _mean;
	std_dev = _std_dev;
	
	const float radius = current_size/2;
	
	// for clarity
	const float s = std_dev->getValue().getFloat();
	const float u = mean->getValue().getFloat();
	const float pi = M_PI;
	
	// leave the one pixel margin around the edge
	for(unsigned int channel=0; channel<M_MASK_CHANNELS; ++channel) {
		for(unsigned int row=0; row<current_size; ++row) {
			for(unsigned int column=0; column<current_size; ++column) {
				const float d = sqrt(((row-radius)*(row-radius)) + ((column-radius)*(column-radius)))/radius;
				
				const GLfloat value = (1/(s*sqrt(2*pi)))*exp(-1*(d-u)*(d-u)/(2*s*s));
				data[channel+M_MASK_CHANNELS*current_size*row+M_MASK_CHANNELS*column] = value;
			}
		}
	}
}

const std::string GaussianMask::getName() const {
	return "gaussian";
}


