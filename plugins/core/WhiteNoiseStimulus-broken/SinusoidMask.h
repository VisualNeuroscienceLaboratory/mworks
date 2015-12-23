/*
 *  SinusoidDriftingGratingMask.h
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef SINUSOID_MASK_H
#define SINUSOID_MASK_H

#include "Mask.h"
using namespace mw;

class SinusoidMask : public Mask {
public:
    SinusoidMask(const shared_ptr<Variable> &_size);    
    virtual const std::string getName() const;
};

#endif
