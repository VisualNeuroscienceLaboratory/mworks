/*
 *  GammaStimulus.cpp
 *  MWorksCore
 *
 *  Created by romesh kumbhani on 08/06/15.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#include "GammaStimulus.h"

#include <MWorksCore/StandardStimuli.h>

#include <algorithm>



const std::string GammaStimulus::R_GAMMA("r_gamma");
const std::string GammaStimulus::G_GAMMA("g_gamma");
const std::string GammaStimulus::B_GAMMA("b_gamma");


void GammaStimulus::describeComponent(ComponentInfo &info) {
    StandardDynamicStimulus::describeComponent(info);
    
    info.setSignature("stimulus/gamma");

    info.addParameter(R_GAMMA, "1.0");
    info.addParameter(G_GAMMA, "1.0");
    info.addParameter(B_GAMMA, "1.0");

}


GammaStimulus::GammaStimulus(const ParameterValueMap &parameters) :
    StandardDynamicStimulus(parameters),
    r_gamma(registerVariable(parameters[R_GAMMA])),
    g_gamma(registerVariable(parameters[B_GAMMA])),
    b_gamma(registerVariable(parameters[G_GAMMA]))
{
    CGDirectDisplayID activeDisplays[2];
    CGGetActiveDisplayList(maxDisplays, activeDisplays, &displayCount);
    mprintf("GammaStimulus: %d displays found",displayCount);
    mainMonID = CGMainDisplayID();
    stimMonID = mainMonID;
    
    for (uint8 ii=0;ii<displayCount;ii++)
    {
        mprintf("GammaStimulus: Display %d ID: %u",ii,activeDisplays[ii]);
        if (mainMonID != activeDisplays[ii])
            stimMonID = activeDisplays[ii];
            
    }
    mprintf("GammaStimulus: Main Display ID: %u",mainMonID);
    mprintf("GammaStimulus: Stimulus Display ID: %u",stimMonID);

    r_Min = 0.0; r_Max = 1.0;
    g_Min = 0.0; g_Max = 1.0;
    b_Min = 0.0; b_Max = 1.0;

    old_r_Min = 0.0; old_r_Max = 1.0; old_r_gamma = 1.0;
    old_g_Min = 0.0; old_g_Max = 1.0; old_g_gamma = 1.0;
    old_b_Min = 0.0; old_b_Max = 1.0; old_b_gamma = 1.0;
    
}

GammaStimulus::~GammaStimulus()
{
    float tmp_r_Min, tmp_r_Max, tmp_r_gamma;
    float tmp_g_Min, tmp_g_Max, tmp_g_gamma;
    float tmp_b_Min, tmp_b_Max, tmp_b_gamma;
    
    CGGetDisplayTransferByFormula(stimMonID,
                                  &tmp_r_Min, &tmp_r_Max, &tmp_r_gamma,
                                  &tmp_g_Min, &tmp_g_Max, &tmp_g_gamma,
                                  &tmp_b_Min, &tmp_b_Max, &tmp_b_gamma);
    mprintf("GammaStiulus: CURRENT VALUES: r:[%f %f %f], g:[%f %f %f], b:[%f %f %f]",
            tmp_r_Min, tmp_r_Max, tmp_r_gamma,
            tmp_g_Min, tmp_g_Max, tmp_g_gamma,
            tmp_b_Min, tmp_b_Max, tmp_b_gamma);
    
    CGSetDisplayTransferByFormula(stimMonID,
                                  old_r_Min, old_r_Max, old_r_gamma,
                                  old_g_Min, old_g_Max, old_g_gamma,
                                  old_b_Min, old_b_Max, old_b_gamma);
    mprintf("GammaStiulus: SET TO OLD VALUES: r:[%f %f %f], g:[%f %f %f], b:[%f %f %f]",
            old_r_Min, old_r_Max, old_r_gamma,
            old_g_Min, old_g_Max, old_g_gamma,
            old_b_Min, old_b_Max, old_b_gamma);

}


void GammaStimulus::load(shared_ptr<StimulusDisplay> display) {

    if (loaded)
        return;
    
    CGGetDisplayTransferByFormula(stimMonID,
                                  &old_r_Min, &old_r_Max, &old_r_gamma,
                                  &old_g_Min, &old_g_Max, &old_g_gamma,
                                  &old_b_Min, &old_b_Max, &old_b_gamma);
    mprintf("GammaStiulus: OLD VALUES: r:[%f %f %f], g:[%f %f %f], b:[%f %f %f]",
            old_r_Min, old_r_Max, old_r_gamma,
            old_g_Min, old_g_Max, old_g_gamma,
            old_b_Min, old_b_Max, old_b_gamma);

    CGSetDisplayTransferByFormula(stimMonID,
                                  r_Min, r_Max, 1.0f/r_gamma->getValue().getFloat(),
                                  g_Min, g_Max, 1.0f/g_gamma->getValue().getFloat(),
                                  b_Min, b_Max, 1.0f/b_gamma->getValue().getFloat());

    mprintf("GammaStiulus: NEW VALUES: r:[%f %f %f], g:[%f %f %f], b:[%f %f %f]",
            r_Min, r_Max, 1.0f/(r_gamma->getValue().getFloat()),
            g_Min, g_Max, 1.0f/(g_gamma->getValue().getFloat()),
            b_Min, b_Max, 1.0f/(b_gamma->getValue().getFloat()));
    
    
    loaded = true;
}   


void GammaStimulus::unload(shared_ptr<StimulusDisplay> display) {
    if (!loaded)
        return;

    float tmp_r_Min, tmp_r_Max, tmp_r_gamma;
    float tmp_g_Min, tmp_g_Max, tmp_g_gamma;
    float tmp_b_Min, tmp_b_Max, tmp_b_gamma;
    
    CGGetDisplayTransferByFormula(stimMonID,
                                  &tmp_r_Min, &tmp_r_Max, &tmp_r_gamma,
                                  &tmp_g_Min, &tmp_g_Max, &tmp_g_gamma,
                                  &tmp_b_Min, &tmp_b_Max, &tmp_b_gamma);
    mprintf("GammaStiulus: CURRENT VALUES: r:[%f %f %f], g:[%f %f %f], b:[%f %f %f]",
            tmp_r_Min, tmp_r_Max, tmp_r_gamma,
            tmp_g_Min, tmp_g_Max, tmp_g_gamma,
            tmp_b_Min, tmp_b_Max, tmp_b_gamma);

    CGSetDisplayTransferByFormula(stimMonID,
                                  old_r_Min, old_r_Max, old_r_gamma,
                                  old_g_Min, old_g_Max, old_g_gamma,
                                  old_b_Min, old_b_Max, old_b_gamma);
    mprintf("GammaStiulus: SET TO OLD VALUES: r:[%f %f %f], g:[%f %f %f], b:[%f %f %f]",
            old_r_Min, old_r_Max, old_r_gamma,
            old_g_Min, old_g_Max, old_g_gamma,
            old_b_Min, old_b_Max, old_b_gamma);
    
    loaded = false;
}


void GammaStimulus::drawFrame(shared_ptr<StimulusDisplay> display) {
    return;
}


Datum GammaStimulus::getCurrentAnnounceDrawData() {
	boost::mutex::scoped_lock locker(stim_lock);
	Datum announce_data = StandardDynamicStimulus::getCurrentAnnounceDrawData();

	announce_data.addElement(STIM_TYPE, "gamma");
    announce_data.addElement("r_gamma",              r_gamma->getValue());
    announce_data.addElement("g_gamma",              g_gamma->getValue());
    announce_data.addElement("b_gamma",              b_gamma->getValue());

    return announce_data;
}

























