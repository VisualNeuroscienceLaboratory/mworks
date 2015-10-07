/*
 *  UniformGratingData.cpp
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "UniformGratingData.h"


UniformGratingData::UniformGratingData(const shared_ptr <Variable> &_data_sample_rate) : GratingData(_data_sample_rate) {
    for(unsigned int i=0; i<current_data_size; ++i) {
        // use cos so it matches other gratings
        data[i] = 1.0f;
    }
}

const std::string UniformGratingData::getName() const {
    return "uniform";
}


