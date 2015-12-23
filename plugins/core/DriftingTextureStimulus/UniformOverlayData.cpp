/*
 *  UniformOverlayData.cpp
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#include "UniformOverlayData.h"
using namespace mw;

UniformOverlayData::UniformOverlayData(const shared_ptr<Variable> &_size) : OverlayData(_size){
    
    // leave the one pixel margin around the edge
    for(unsigned int channel=0; channel<M_OVERLAY_CHANNELS; ++channel) {
        for(unsigned int row=0; row<current_size; ++row) {
            for(unsigned int column=0; column<current_size; ++column) {
                data[channel+M_OVERLAY_CHANNELS*current_size*row+M_OVERLAY_CHANNELS*column] = 1.0f;
            }
        }
    }
}


const std::string UniformOverlayData::getName() const {
    return "uniform";
}
