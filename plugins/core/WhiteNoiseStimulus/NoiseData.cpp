/*
 *  NoiseData.cpp
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by romesh kumbhani on 2015-10-07.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */



#include "NoiseData.h"
#include "Utilities.h"

#include <algorithm>

using namespace mw;

NoiseData::NoiseData(string distribution, uint _nsubchecks_x, uint _nsubchecks_y, uint _nchecks_x, uint _nchecks_y,
                     shared_ptr<Variable> seed, float _dmu, float _drange) {
    //data_size = std::max(2u, Utilities::getNextPowerOfTwo());

    nsubchecks_x = _nsubchecks_x;
    nsubchecks_y = _nsubchecks_y;
    nchecks_x = _nchecks_x;
    nchecks_y = _nchecks_y;
    ntotalcols = _nsubchecks_x * nchecks_x;
    ntotalrows = _nsubchecks_y * nchecks_y;
    datasize = ntotalrows * ntotalcols * M_MASK_CHANNELS;
    data = new GLfloat[datasize];
    blockdatasize = nchecks_x*nchecks_y;
    blockdata = new GLfloat[blockdatasize];
    dtype = distribution;
    dmu = _dmu;
    drange = _drange;
    srand(seed->getValue().getInteger());
    genNewData();
    
}

NoiseData::~NoiseData() {
	delete [] data;
}


void NoiseData::genNewData(){
    uint cnt = 0;
    for(uint r=0; r<ntotalrows; r += nsubchecks_y) {
        for(uint c=0; c<ntotalcols; c += nsubchecks_x) {
            GLfloat val = getRandVal();
            blockdata[cnt] = val;
            cnt++;
            uint basendx = (r*ntotalcols + c);
            for(uint sr=0; sr<nsubchecks_y; ++sr) {
                for(uint sc=0; sc<nsubchecks_x; ++sc) {
                    for(unsigned int channel=0; channel<M_MASK_CHANNELS; ++channel) {
                        data[(basendx + (sr*ntotalcols) + sc)*M_MASK_CHANNELS+channel] = val;
                    }
                }
            }
        }
    }
    fnum++;
}


const GLfloat * NoiseData::getData() const {
	return data;
}

const GLfloat * NoiseData::getBlockData() const {
    return blockdata;
}

unsigned int NoiseData::getSizeW() const {
    return ntotalcols;
}

unsigned int NoiseData::getSizeH() const {
    return ntotalrows;
}

unsigned int NoiseData::getSizeT() const {
    return datasize;
}

unsigned int NoiseData::getSizeBT() const {
    return blockdatasize;
}

unsigned int NoiseData::getFrame() const {
    return fnum;
}

const std::string NoiseData::getName() const {
    return dtype;
}

GLfloat NoiseData::getRandVal(){
    static double tmp = 0.0;
    double rval = 0.5;
    if (dtype == "ternary"){
        rval = (double)((rand() % 3) / 2.0);
    }
    else if (dtype == "binary"){
        rval = (double)(rand() % 2);
    }
    else if (dtype == "gaussian"){
        double U, V;
        U = rand()/RAND_MAX;
        V = rand()/RAND_MAX;
        
        rval = (double)((sqrt(-2 * log(U)) * sin(2.0 * M_PI * V))*(drange/6.0)+((dmu+1.0)/2.0));
    }
    else if (dtype == "test"){
        rval = tmp;
        tmp += 0.1; //1.0/(51.0*3);
        if (tmp > 1.0)
            tmp = 0.0;
    }else if(dtype == "unique"){
        rval = 0.12;
    }
        
    rval = (std::min(1.0,std::max(0.0,rval)));
    return (GLfloat)rval;
}