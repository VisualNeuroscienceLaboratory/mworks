/*
 *  FlattopWhiteNoiseMask.h
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef FLATTOP_MASK_H
#define FLATTOP_MASK_H

#include "Mask.h"
using namespace mw;

class FlattopMask : public Mask {
public:
    FlattopMask(const shared_ptr<Variable> &_size, uint8_t n);
    
    virtual const std::string getName() const;
};

#endif
