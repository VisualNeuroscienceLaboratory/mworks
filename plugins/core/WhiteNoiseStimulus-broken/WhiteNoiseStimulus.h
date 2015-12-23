/*
 *  WhiteNoiseStimulus.h
 *
 *  Created by romesh kumbhani on 10/6/15
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
    std::shared_ptr<NoiseData> noise;
    std::shared_ptr<Mask>      mask;
    vector<GLuint>             noise_textures;
    vector<GLuint>             mask_textures;
    
    std::shared_ptr<Variable>  scrx;
    std::shared_ptr<Variable>  scry;
    std::shared_ptr<Variable>  rotation;
    std::shared_ptr<Variable>  opacity;
    std::shared_ptr<Variable>  check_size_x;
    std::shared_ptr<Variable>  check_size_y;
    std::shared_ptr<Variable>  n_checks_x;
    std::shared_ptr<Variable>  n_checks_y;
    std::shared_ptr<Variable>  dist_mu;
    std::shared_ptr<Variable>  dist_rng;

public:
    static const std::string DISTRIBUTION_TYPE;
    static const std::string MASK;
    static const std::string SCRX;
    static const std::string SCRY;
    static const std::string ROTATION;
    static const std::string OPACITY;
    static const std::string CHECK_SIZE_X;
    static const std::string CHECK_SIZE_Y;
    static const std::string N_CHECK_X;
    static const std::string N_CHECK_Y;
    static const std::string DIST_MU;
    static const std::string DIST_RNG;
    static const std::string MEAN;
    static const std::string STD_DEV;
    
    explicit     WhiteNoiseStimulus(const ParameterValueMap &parameters);
    virtual      ~WhiteNoiseStimulus() { }
    static  void describeComponent(ComponentInfo &info);
	virtual void load(shared_ptr<StimulusDisplay> display);
	virtual void unload(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display);
	virtual      Datum getCurrentAnnounceDrawData();

};


#endif 





















