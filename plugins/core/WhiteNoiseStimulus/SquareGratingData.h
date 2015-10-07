/*
 *  SquareGratingData.h
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by bkennedy on 11/7/08.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef SQUARE_GRATING_DATA_H
#define SQUARE_GRATING_DATA_H

#include "GratingData.h"

class SquareGratingData : public GratingData {
public:
	SquareGratingData(const shared_ptr <Variable> &_data_sample_size);
	const std::string getName() const;
};

#endif 

