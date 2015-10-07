/*
 *  TriangleGratingData.h
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by bkennedy on 11/13/08.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef TRIANGLE_GRATING_DATA_H
#define TRIANGLE_GRATING_DATA_H

#include "GratingData.h"

class TriangleGratingData : public GratingData {
public:
	TriangleGratingData(const shared_ptr <Variable> &_data_sample_size);
	const std::string getName() const;
};

#endif 
