/*
 *  OverlayData.h
 *  DriftingGratingStimulusPlugin
 *
 *  Created by rdk on 7/10/15.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#ifndef OVERLAY_DATA_H
#define OVERLAY_DATA_H

#include "MWorksCore/GenericVariable.h"

using namespace mw;

#define M_OVERLAY_CHANNELS 2

class OverlayData {
protected:
    GLfloat *data;
    boost::shared_ptr<Variable> size;
    unsigned int current_size;
    
    
public:
    OverlayData(const shared_ptr<Variable> &_size);
    virtual ~OverlayData();
    GLint getSize() const;
    const GLfloat *get2DData() const;
    virtual const std::string getName() const = 0;
};

#endif