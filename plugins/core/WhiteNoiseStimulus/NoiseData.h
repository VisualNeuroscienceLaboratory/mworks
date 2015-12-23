/*
 *  NoiseData.h
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by romesh kumbhani on 2015-10-07.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#ifndef NOISE_DATA_H
#define NOISE_DATA_H

#include "MWorksCore/GenericVariable.h"

using namespace mw;

#define M_MASK_CHANNELS 2

class NoiseData {
protected:
	GLfloat                  *data;
    uint                      datasize;
    GLfloat                  *blockdata;
    uint                      blockdatasize;

    uint                      nsubchecks_x;
    uint                      nsubchecks_y;
    uint                      nchecks_x;
    uint                      nchecks_y;
    uint                      ntotalcols;
    uint                      ntotalrows;

    string                    dtype;
    float                     dmu    = 0;
    float                     drange = 1;
    GLfloat                   getRandVal();
    
    uint                      fnum = 0;

public:
    NoiseData(string distribution,
              uint nsubchecks_x, uint nsubchecks_y,
              uint nchecks_x, uint nchecks_y,
              shared_ptr<Variable> seed,
              float _dmu, float _drange);
	virtual                  ~NoiseData();
    virtual void              genNewData();
	virtual const GLfloat    *getData() const;
    virtual const GLfloat    *getBlockData() const;
	virtual unsigned int      getSizeW() const;
    virtual unsigned int      getSizeH() const;
    virtual unsigned int      getSizeT() const;
    virtual unsigned int      getSizeBT() const;
    virtual unsigned int      getFrame() const;
	virtual const string      getName() const;
};

#endif 

