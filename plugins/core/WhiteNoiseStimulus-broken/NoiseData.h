/*
 *  NoiseData.h
 *
 *  Created by Romesh Kumbhani on 10/6/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */


#ifndef NOISE_DATA_H
#define NOISE_DATA_H

#include "MWorksCore/GenericVariable.h"
#include <string>
using namespace std;
using namespace mw;

class NoiseData {
protected:
    string DistributionType;
	GLfloat *data;
    unsigned int datasize[2] = {1,1};
    unsigned int totalsize = 1;
public:
	NoiseData(uint nChecksX,uint nChecksY, uint pixPerCheck);
	virtual ~NoiseData();
	virtual const GLfloat *   get1DData() const;
	virtual unsigned int      getSize() const;
	const std::string         getName() const;
};

#endif 
