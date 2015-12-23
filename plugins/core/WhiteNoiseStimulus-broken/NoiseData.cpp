/*
 *  NoiseData.cpp
 *
 *  Created by Romesh Kumbhani on 10/6/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */



#include "NoiseData.h"
#include "Support.h"

#include <algorithm>

using namespace mw;

NoiseData::NoiseData(uint nChecksX, uint nChecksY, uint pixPerCheck) {
    datasize[0] = nChecksX*pixPerCheck;
    datasize[1] = nChecksY*pixPerCheck;
    
    totalsize = (datasize[0]*datasize[1]);
    data = new GLfloat[totalsize];
}

NoiseData::~NoiseData() {
	delete [] data;
}

const GLfloat * NoiseData::get1DData() const {
	return data;
}

unsigned int NoiseData::getSize() const {
	return totalsize;
}

const std::string NoiseData::getName() const {
    return DistributionType;
}