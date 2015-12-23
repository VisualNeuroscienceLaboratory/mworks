/*
 *  GammaStimulus.h
 *  MWorksCore
 *
 *  Created by romesh kumbhani on 08/06/15.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#ifndef GAMMA_STIMULUS_H
#define GAMMA_STIMULUS_H

#include <MWorksCore/StandardDynamicStimulus.h>

using namespace mw;


class GammaStimulus : public StandardDynamicStimulus {

private:
    float r_Min, r_Max;
    float g_Min, g_Max;
    float b_Min, b_Max;
    shared_ptr<Variable> r_gamma;
    shared_ptr<Variable> g_gamma;
    shared_ptr<Variable> b_gamma;

    float old_r_Min, old_r_Max, old_r_gamma;
    float old_g_Min, old_g_Max, old_g_gamma;
    float old_b_Min, old_b_Max, old_b_gamma;

    
    typedef uint32_t CGDirectDisplayID;
    uint32_t maxDisplays = 2;
    uint32_t displayCount;
    
    CGDirectDisplayID mainMonID;
    CGDirectDisplayID stimMonID;



public:
    static const std::string R_GAMMA;
    static const std::string G_GAMMA;
    static const std::string B_GAMMA;
    
    
    static void describeComponent(ComponentInfo &info);
    
    explicit GammaStimulus(const ParameterValueMap &parameters);
    
    virtual ~GammaStimulus();

	virtual void load(shared_ptr<StimulusDisplay> display);
	virtual void unload(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display);
	virtual Datum getCurrentAnnounceDrawData();

};


#endif 





















