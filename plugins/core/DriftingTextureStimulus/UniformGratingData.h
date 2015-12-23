/*
 *  UniformGratingData.h
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef UNIFORM_GRATING_DATA_H
#define UNIFORM_GRATING_DATA_H

#include "GratingData.h"

class UniformGratingData : public GratingData {
public:
    UniformGratingData(const shared_ptr <Variable> &_data_sample_size);
    const std::string getName() const;
};

#endif
