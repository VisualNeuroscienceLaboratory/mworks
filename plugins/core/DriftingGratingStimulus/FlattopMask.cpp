/*
 *  FlattopDriftingGratingMask.cpp
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "FlattopMask.h"
using namespace mw;

FlattopMask::FlattopMask(const shared_ptr<Variable> &_size, uint8_t n) : Mask(_size){

    const float radius = current_size/2;
    float alpha = 1.0f;
    
    // for clarity
    const float base = (2.0f)/((float)(n));
    const float s =        base/3.0f;
    const float u = 1.0f - base;
    
    unsigned int r,c;
    
    // leave the one pixel margin around the edge
    for(unsigned int channel=0; channel<M_MASK_CHANNELS; ++channel) {
        for(unsigned int row=0; row<current_size; ++row) {
            for(unsigned int column=0; column<current_size; ++column) {
                r = row-radius;
                c = column-radius;
                const float d = sqrt((r*r) + (c*c))/radius;
                if (fabs(d)<u)
                    data[channel+M_MASK_CHANNELS*current_size*row+M_MASK_CHANNELS*column] = alpha;
                else
                    data[channel+M_MASK_CHANNELS*current_size*row+M_MASK_CHANNELS*column] = alpha*exp(-0.5*(d-u)*(d-u)/(s*s));
            }
        }
    }
}


const std::string FlattopMask::getName() const {
    return "flattop";
}





