/*
 *  WhiteNoiseStimulus.h
 *  MWorksCore
 *
 *  Created by romesh kumbhani on 2015-10-07.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#ifndef WHITE_NOISE_STIMULUS_H
#define WHITE_NOISE_STIMULUS_H

#include <MWorksCore/StandardDynamicStimulus.h>

#include "Mask.h"
#include "NoiseData.h"

using namespace mw;


class WhiteNoiseStimulus : public StandardDynamicStimulus {

private:
    shared_ptr<NoiseData>   noise;
    shared_ptr<Mask>        mask;
    vector<GLuint>          noise_textures;
    vector<GLuint>          mask_textures;

    string                  noise_str;
    string                  mask_str;
    
    shared_ptr<Variable>    width;
    shared_ptr<Variable>    height;
    shared_ptr<Variable>    x_position;
	shared_ptr<Variable>    y_position;
    shared_ptr<Variable>    rotation;
	
    shared_ptr<Variable>    opacity;
    shared_ptr<Variable>    nchecks_x;
    shared_ptr<Variable>    nchecks_y;

    shared_ptr<Variable>    distrib_mu;
    shared_ptr<Variable>    distrib_range;
    shared_ptr<Variable>    nframes;
    shared_ptr<Variable>    seed;

    CGDirectDisplayID       stimMonID;
    float                   pixperdeg;
    float                   degperpix;
    uint                    curframe = 1;
    
    uint                    nsubchecks_x;
    uint                    nsubchecks_y;
    
    float                   width_deg;
    float                   height_deg;
    
    
    

public:
    static const std::string NOISE;
    static const std::string MASK;
    
    static const std::string OPACITY;

    static const std::string NCHECKS_X;
    static const std::string NCHECKS_Y;
    
    static const std::string DISTRIB_MU;
    static const std::string DISTRIB_RANGE;
    
    static const std::string NFRAMES;
    static const std::string SEED;

    // for use with Gaussian Masks:
    static const std::string MEAN;
    static const std::string STD_DEV;
    
    static void describeComponent(ComponentInfo &info);
    
    explicit WhiteNoiseStimulus(const ParameterValueMap &parameters);
    
	virtual ~WhiteNoiseStimulus() { }

	virtual void load(shared_ptr<StimulusDisplay> display);
    
	virtual void unload(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display);
	virtual Datum getCurrentAnnounceDrawData();

};


#endif 





















