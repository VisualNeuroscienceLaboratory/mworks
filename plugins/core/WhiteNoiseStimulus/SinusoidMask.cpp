/*
 *  SinusoidMask.cpp
 *  StimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "SinusoidMask.h"
using namespace mw;

SinusoidMask::SinusoidMask(const shared_ptr<Variable> &_size) : Mask(_size){
    
    // leave the one pixel margin around the edge
    for(unsigned int channel=0; channel<M_MASK_CHANNELS; ++channel) {
        for(unsigned int row=0; row<current_size; ++row) {
            for(unsigned int column=0; column<current_size; ++column) {
                data[channel+M_MASK_CHANNELS*current_size*row+M_MASK_CHANNELS*column] = 0.5f*(1+cos(2*M_PI*column/current_size));
            }
        }
    }
}


const std::string SinusoidMask::getName() const {
    return "sinusoid";
}





