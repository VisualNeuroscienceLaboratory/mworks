/*
 ImageData.h, rdk 9/15/2015
 */

#ifndef DRIFTING_IMAGE_STIMULUS_H
#define DRIFTING_IMAGE_STIMULUS_H

#include <MWorksCore/StandardDynamicStimulus.h>

#include "Mask.h"
#include "ImageData.h"
#include "OverlayData.h"

using namespace mw;


class DriftingImageStimulus : public StandardDynamicStimulus {

private:
    shared_ptr<ImageData> img;
    shared_ptr<OverlayData> overlay;
    shared_ptr<Mask> mask;
    vector<GLuint> image_textures;
    vector<GLuint> overlay_textures;
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
	
    shared_ptr<Variable> o_rotation;
    shared_ptr<Variable> o_direction_in_degrees;
    shared_ptr<Variable> o_spatial_frequency;
    shared_ptr<Variable> o_temporal_frequency;
    shared_ptr<Variable> o_starting_phase;
    
    
    
	float last_phase;
    float o_last_phase;

public:
    static const std::string IMAGE_TYPE;
    static const std::string MASK;
    static const std::string OVERLAY_TYPE;

    static const std::string IMAGE_SAMPLE_RATE;
    static const std::string INVERTED;
    static const std::string STD_DEV;
    static const std::string MEAN;

    static const std::string CONTRAST;
    static const std::string OPACITY;
    
    static const std::string DIRECTION;
    static const std::string SPATIAL_FREQUENCY;
    static const std::string TEMPORAL_FREQUENCY;
    static const std::string STARTING_PHASE;
    
    static const std::string O_ROTATION;
    static const std::string O_DIRECTION;
    static const std::string O_SPATIAL_FREQUENCY;
    static const std::string O_TEMPORAL_FREQUENCY;
    static const std::string O_STARTING_PHASE;
    
    
    static void describeComponent(ComponentInfo &info);
    
    explicit DriftingImageStimulus(const ParameterValueMap &parameters);
    
	virtual ~DriftingImageStimulus() { }

	virtual void load(shared_ptr<StimulusDisplay> display);
	virtual void unload(shared_ptr<StimulusDisplay> display);
    virtual void drawFrame(shared_ptr<StimulusDisplay> display);
	virtual Datum getCurrentAnnounceDrawData();

};


#endif 





















