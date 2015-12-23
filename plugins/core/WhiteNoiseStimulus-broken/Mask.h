/*
 *  Mask.h
 *
 *  Created by bkennedy on 11/13/08.
 *  Copyright 2008 mit. All rights reserved.
 *
 *  Modified by romesh kumbhani on 10/6/15.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#ifndef MASK_H
#define MASK_H

#include "MWorksCore/GenericVariable.h"

using namespace mw;

#define M_MASK_CHANNELS 2
#define M_MIN_MASK_SIZE 4u
class Mask {
protected:
	GLfloat *data;
	boost::shared_ptr<Variable> size;
	unsigned int current_size;
	
	
public:
	Mask(const shared_ptr<Variable> &_size);
	virtual ~Mask();
	GLint getSize() const;
	const GLfloat *getData() const;
	virtual const std::string getName() const = 0;
};

#endif 
