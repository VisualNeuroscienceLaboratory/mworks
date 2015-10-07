/*
 *  SinusoidGratingData.h
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by bkennedy on 11/7/08.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef SINUSOID_GRATING_DATA_H
#define SINUSOID_GRATING_DATA_H

#include "GratingData.h"

class SinusoidGratingData : public GratingData {
public:
    SinusoidGratingData(const shared_ptr <Variable> &_data_sample_size);
    const std::string getName() const;
};

#endif