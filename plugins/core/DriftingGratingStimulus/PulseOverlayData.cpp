/*
 *  PulseOverlayData.cpp
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#include "PulseOverlayData.h"
using namespace mw;

PulseOverlayData::PulseOverlayData(const shared_ptr<Variable> &_size) : OverlayData(_size){
    
    // leave the one pixel margin around the edge
    for(unsigned int channel=0; channel<M_OVERLAY_CHANNELS; ++channel) {
        for(unsigned int row=0; row<current_size; ++row) {
            for(unsigned int column=0; column<current_size; ++column) {
                data[channel+M_OVERLAY_CHANNELS*current_size*row+M_OVERLAY_CHANNELS*column] = cos(2*M_PI*column/current_size)>0 ? 1.0f : 0.0f;
            }
        }
    }
}


const std::string PulseOverlayData::getName() const {
    return "pulse";
}
