/*
 GratingData.h, rdk 9/15/2015
 */

#ifndef WHITE_NOISE_STIMULUS_H
#define WHITE_NOISE_STIMULUS_H

#include <MWorksCore/StandardDynamicStimulus.h>

#include "Mask.h"
#include "GratingData.h"

using namespace mw;


class WhiteNoiseStimulus : public StandardDynamicStimulus {

private:
    shared_ptr<GratingData> grating;
    shared_ptr<Mask> mask;
    vector<GLuint> grating_textures;
    vector<GLuint> mask_textures;
    
    
    shared_ptr<Variable> xoffset;
	shared_ptr<Variable> yoffset;
	shared_ptr<Variable> width;
	shared_ptr<Variable> height;
	
    shared_ptr<Variable> contrast;
    shared_ptr<Variable> opacity;
	
    shared_ptr<Variable> rotation;
	shared_ptr<Variable> direction_in_degrees;
	shared_ptr<Variable> spatial_frequency;
    shared_ptr<Variable> temporal_frequency;
	shared_ptr<Variable> starting_phase;
	
	float last_phase;
    float o_last_phase;

public:
    static const std::string GRATING_TYPE;
    static const std::string MASK;

    static const std::string GRATING_SAMPLE_RATE;
    static const std::string INVERTED;
    static const std::string STD_DEV;
    static const std::string MEAN;

    static const std::string CONTRAST;
    static const std::string OPACITY;
    
    static const std::string DIRECTION;
    static const std::string SPATIAL_FREQUENCY;
    static const std::string TEMPORAL_FREQUENCY;
    static const std::string STARTING_PHASE;
    
    static void describeComponent(ComponentInfo &info);
    
    explicit WhiteNoiseStimulus(const ParameterValueMap &parameters);
    
	virtual ~WhiteNoiseStimulus() { }

	virtual void load(shared_ptr<StimulusDisplay> display);
	virtual void unload(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display);
	virtual Datum getCurrentAnnounceDrawData();

};


#endif 





















