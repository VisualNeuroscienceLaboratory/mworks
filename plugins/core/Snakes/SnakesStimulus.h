/*
 *  SnakesStimulus.h
 *  MWorksCore
 *
 *  Created by romesh kumbhani on 2015-10-07.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#ifndef SNAKES_STIMULUS_H
#define SNAKES_STIMULUS_H

#include <MWorksCore/StandardDynamicStimulus.h>
#include <MWorksCore/Stimulus.h>

using namespace mw;


//class SnakesStimulus : public StandardDynamicStimulus {
class SnakesStimulus : public Stimulus {

private:
//    vector<GLuint>          line_textures;
    
    shared_ptr<Variable>    width;
    shared_ptr<Variable>    height;
    shared_ptr<Variable>    x_position;
	shared_ptr<Variable>    y_position;
    shared_ptr<Variable>    rotation;
	
    shared_ptr<Variable>    opacity;
    shared_ptr<Variable>    line_width;
    shared_ptr<Variable>    line_length;
    shared_ptr<Variable>    line_ang;
    shared_ptr<Variable>    shear_ang;
    shared_ptr<Variable>    spacing;
    shared_ptr<Variable>    nsegments;
    shared_ptr<Variable>    jitter;
    
    void drawDaLine(float xpos, float ypos, float angle_degrees, float length, float width, float color, float opa);

public:
    static const std::string OPACITY;

    static const std::string LINE_WIDTH;
    static const std::string LINE_LENGTH;
    static const std::string SHEAR_ANG;
    static const std::string SPACING;
    
    static const std::string NSEGMENTS;
    static const std::string JITTER;
    
    
    static void describeComponent(ComponentInfo &info);
    
    explicit SnakesStimulus(const ParameterValueMap &parameters);
    
	virtual ~SnakesStimulus() { }

	virtual void load(shared_ptr<StimulusDisplay> display);
    
	virtual void unload(shared_ptr<StimulusDisplay> display);
    //virtual void drawFrame(shared_ptr<StimulusDisplay> display);
    virtual void draw(shared_ptr<StimulusDisplay> display);

    virtual Datum getCurrentAnnounceDrawData();

};


#endif 





















