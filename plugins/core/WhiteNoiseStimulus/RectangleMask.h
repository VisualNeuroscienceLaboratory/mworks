/*
 *  RectangleWhiteNoiseMask.h
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by bkennedy on 11/7/08.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#ifndef RECTANGLE_MASK_H
#define RECTANGLE_MASK_H

#include "Mask.h"

class RectangleMask : public Mask {
public:
	RectangleMask(const shared_ptr<Variable> &_size);
	virtual const std::string getName() const;
};

#endif 


