/*
 *  UniformOverlayData.h
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef UNIFORM_OVERLAY_DATA_H
#define UNIFORM_OVERLAY_DATA_H

#include "OverlayData.h"

using namespace mw;

class UniformOverlayData : public OverlayData {
public:
    UniformOverlayData(const shared_ptr<Variable> &_size);
    virtual const std::string getName() const;
};

#endif
