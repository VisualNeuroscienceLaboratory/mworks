/*
 *  SinusoidGratingData.cpp
 *  DriftingGratingStimulusPlugin
 *
 *  Created by bkennedy on 11/7/08.
 *  Copyright 2008 mit. All rights reserved.
 *
 */

#include "SinusoidGratingData.h"


SinusoidGratingData::SinusoidGratingData(const shared_ptr <Variable> &_data_sample_rate) : GratingData(_data_sample_rate) {
    for(unsigned int i=0; i<current_data_size; ++i) {
        // use cos so it matches other gratings
        data[i] = 0.5*(1+cos(2*M_PI*i/current_data_size));
    }
}

const std::string SinusoidGratingData::getName() const {
    return "sinusoid";
}

