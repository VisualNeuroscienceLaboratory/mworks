/*
 *  EllipseMask.h
 *  StimulusPlugin
 *
 *  Created by bkennedy on 11/7/08.
 *  Copyright 2008 mit. All rights reserved.
 *
 */

#ifndef ELLIPSE_MASK_H
#define ELLIPSE_MASK_H

#include "Mask.h"
using namespace mw;

class EllipseMask : public Mask {
public:
	EllipseMask(const shared_ptr<Variable> &_size);
	virtual const std::string getName() const;
};

#endif 
