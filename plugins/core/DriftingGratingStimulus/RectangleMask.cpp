/*
 *  RectangleDriftingGratingMask.cpp
 *  DriftingGratingStimulusPlugin
 *
 *  Created by bkennedy on 11/7/08.
 *  Copyright 2008 mit. All rights reserved.
 *
 */

#include "RectangleMask.h"

RectangleMask::RectangleMask(const shared_ptr<Variable> &_size) : Mask(_size) {
	for(int k=0; k<M_MASK_CHANNELS; ++k) {
		for(int i=0; i<current_size; ++i) {
			for(int j=0; j<current_size; ++j) {
				data[k+M_MASK_CHANNELS*current_size*i+M_MASK_CHANNELS*j] = 1;
			}
		}
	}	
}

const std::string RectangleMask::getName() const {
	return "rectangle";
}

