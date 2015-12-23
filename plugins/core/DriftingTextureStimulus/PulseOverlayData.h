/*
 *  PulseOverlayData.h
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef PULSE_OVERLAY_DATA_H
#define PULSE_OVERLAY_DATA_H

#include "OverlayData.h"

using namespace mw;

class PulseOverlayData : public OverlayData {
public:
    PulseOverlayData(const shared_ptr<Variable> &_size);
    virtual const std::string getName() const;
};

#endif
