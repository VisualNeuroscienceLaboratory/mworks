/*
 *  Mask.cpp
 *
 *  Created by bkennedy on 11/13/08.
 *  Copyright 2008 mit. All rights reserved.
 *
 *  Modified by romesh kumbhani on 10/6/15.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "Mask.h"
#include "Support.h"

#include <algorithm>

Mask::Mask(const shared_ptr<Variable> &_size) {
	size=_size;
	
	current_size = std::max(M_MIN_MASK_SIZE, Support::getNextPowerOfTwo(size->getValue().getInteger()));
	data = new GLfloat[current_size*current_size*M_MASK_CHANNELS];
	
	for(unsigned int channel=0; channel<M_MASK_CHANNELS; ++channel) {
		for(unsigned int row=0; row<current_size; ++row) {
			for(unsigned int column=0; column<current_size; ++column) {
				data[channel+M_MASK_CHANNELS*current_size*row+M_MASK_CHANNELS*column] = 0.0;
			}
		}
	}
}


Mask::~Mask() {
	delete [] data;
}

GLint Mask::getSize() const {
	return current_size;
}

const GLfloat * Mask::getData() const {
	return data;
}

