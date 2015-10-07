/*
 *  OverlayData.cpp
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#include "OverlayData.h"
#include "DriftingGratingUtilities.h"

#include <algorithm>

using namespace mw;

OverlayData::OverlayData(const shared_ptr<Variable> &_size) {
    size=_size;
    
    current_size = std::max(4u, DriftingGratingUtilities::getNextPowerOfTwo(size->getValue().getInteger()));
    data = new GLfloat[current_size*current_size*M_OVERLAY_CHANNELS];
    
    for(unsigned int channel=0; channel<M_OVERLAY_CHANNELS; ++channel) {
        for(unsigned int row=0; row<current_size; ++row) {
            for(unsigned int column=0; column<current_size; ++column) {
                data[channel+M_OVERLAY_CHANNELS*current_size*row+M_OVERLAY_CHANNELS*column] = 0.0;
            }
        }
    }
}

OverlayData::~OverlayData() {
    delete [] data;
}

GLint OverlayData::getSize() const {
    return current_size;
}

const GLfloat * OverlayData::get2DData() const {
    return data;
}

const std::string OverlayData::getName() const {
    return "none";
}
