/*
 *  WhiteNoiseStimulus.cpp
 *
 *  Created by romesh kumbhani on 10/6/15
 *  Copyright 2015 NYU. All rights reserved.
 *
 */


#include "WhiteNoiseStimulus.h"

#include <algorithm>
#include <MWorksCore/StandardStimuli.h>
#include "EllipseMask.h"
#include "FlattopMask.h"
#include "GaussianMask.h"
#include "RectangleMask.h"
#include "SinusoidMask.h"

// SETUP VARIABLES

const std::string WhiteNoiseStimulus::DISTRIBUTION_TYPE("distribution_type");
const std::string WhiteNoiseStimulus::MASK("mask");
const std::string WhiteNoiseStimulus::SCRX("scrx");
const std::string WhiteNoiseStimulus::SCRY("scry");
const std::string WhiteNoiseStimulus::ROTATION("rotation");
const std::string WhiteNoiseStimulus::OPACITY("opacity");
const std::string WhiteNoiseStimulus::CHECK_SIZE_X("check_size_x");
const std::string WhiteNoiseStimulus::CHECK_SIZE_Y("check_size_y");
const std::string WhiteNoiseStimulus::N_CHECK_X("n_check_x");
const std::string WhiteNoiseStimulus::N_CHECK_Y("n_check_y");
const std::string WhiteNoiseStimulus::DIST_MU("dist_mu");
const std::string WhiteNoiseStimulus::DIST_RNG("dist_rng");
const std::string WhiteNoiseStimulus::MEAN("mean");
const std::string WhiteNoiseStimulus::STD_DEV("std_dev");


// CONSTRUCTOR

WhiteNoiseStimulus::WhiteNoiseStimulus(const ParameterValueMap &parameters) :
    StandardDynamicStimulus(parameters),
    scrx(registerVariable(parameters[SCRX])),
    scry(registerVariable(parameters[SCRY])),
    rotation(registerVariable(parameters[ROTATION])),
    opacity(registerVariable(parameters[OPACITY])),
    check_size_x(registerVariable(parameters[CHECK_SIZE_X])),
    check_size_y(registerVariable(parameters[CHECK_SIZE_Y])),
    n_check_x(registerVariable(parameters[N_CHECK_X])),
    n_check_y(registerVariable(parameters[N_CHECK_Y])),
    dist_mu(registerVariable(parameters[DIST_MU])),
    dist_rng(registerVariable(parameters[DIST_RNG]))
{
/*
    const std::string &grating_type = parameters[DISTRIBUTION_TYPE].str();
    shared_ptr<Variable> grating_sample_rate(parameters[GRATING_SAMPLE_RATE]);
    

	if (grating_type == "sinusoid") {
        grating = shared_ptr<SinusoidGratingData>(new SinusoidGratingData(grating_sample_rate));
    } else if (grating_type == "square") {
		grating = shared_ptr<SquareGratingData>(new SquareGratingData(grating_sample_rate));
	} else if (grating_type == "triangle") {
		grating = shared_ptr<TriangleGratingData>(new TriangleGratingData(grating_sample_rate));
	} else if (grating_type == "sawtooth") {
		grating = shared_ptr<SawtoothGratingData>(new SawtoothGratingData(grating_sample_rate, VariablePtr(parameters[INVERTED])));
    } else if (grating_type == "uniform") {
        grating = shared_ptr<UniformGratingData>(new UniformGratingData(grating_sample_rate));
	} else {
		throw SimpleException("illegal grating type", grating_type);		
	}
*/
    const std::string &mask_type = parameters[MASK].str();
	shared_ptr<Variable> mask_size(new ConstantVariable(128L));

	if (mask_type == "rectangle") {
		mask = shared_ptr<Mask>(new RectangleMask(mask_size));
	} else if (mask_type == "ellipse") {
		mask = shared_ptr<Mask>(new EllipseMask(mask_size));
    } else if (mask_type == "flattop") {
        mask = shared_ptr<Mask>(new FlattopMask(mask_size,8));
    } else if (mask_type == "flattop8") {
        mask = shared_ptr<Mask>(new FlattopMask(mask_size,8));
    } else if (mask_type == "flattop4") {
        mask = shared_ptr<Mask>(new FlattopMask(mask_size,4));
	} else if (mask_type == "gaussian") {
		mask = shared_ptr<Mask>(new GaussianMask(mask_size, VariablePtr(parameters[MEAN]), VariablePtr(parameters[STD_DEV])));
    } else if (mask_type == "sinusoid") {
        mask = shared_ptr<Mask>(new SinusoidMask(mask_size));
	} else {
		throw SimpleException("illegal mask", mask_type);				
	}
    
}

// DESTRUCTOR

// using default


// METHODS

void WhiteNoiseStimulus::describeComponent(ComponentInfo &info) {
    StandardDynamicStimulus::describeComponent(info);
    
    info.setSignature("stimulus/drifting_grating");
    
    info.addParameter(BasicTransformStimulus::X_SIZE);
    info.addParameter(BasicTransformStimulus::Y_SIZE);
    info.addParameter(BasicTransformStimulus::X_POSITION, "0.0");
    info.addParameter(BasicTransformStimulus::Y_POSITION, "0.0");
    info.addParameter(BasicTransformStimulus::ROTATION, "0.0");
    
    info.addParameter(DISTRIBUTION_TYPE,    "ternary");
    info.addParameter(MASK,                 "flattop8");
    
    info.addParameter(DIST_MU,               "0.0");
    info.addParameter(DIST_RNG,              "1.0");
    
    info.addParameter(OPACITY,              "1.0");
    info.addParameter(OPACITY,              "1.0");
    info.addParameter(OPACITY,              "1.0");
    info.addParameter(OPACITY,              "1.0");
    info.addParameter(OPACITY,              "1.0");
    
    
}

void WhiteNoiseStimulus::load(shared_ptr<StimulusDisplay> display) {
//    typedef uint32_t CGDirectDisplayID;
//    uint32_t maxDisplays = 2;
//    CGDirectDisplayID mainMonID = CGMainDisplayID();
//    CGDirectDisplayID activeDisplays[2];
//    uint32_t displayCount;
//    CGGetActiveDisplayList(maxDisplays, activeDisplays, &displayCount);
//    CGDirectDisplayID stimulusMonID = activeDisplays[1];
//    
//    mprintf("VNL: %d displays found",displayCount);
//    for (uint8 ii=0;ii<displayCount;ii++)
//        mprintf("Display %d ID: %u",ii,activeDisplays[ii]);
//    mprintf("Main Display ID: %u",mainMonID);
//    mprintf("Stimulus Display ID: %u",stimulusMonID);
//    
//    
//    //uint32_t thedisplay = CGMainDisplayID();
//    float redMin,redMax,redGamma;
//    float greenMin,greenMax,greenGamma;
//    float blueMin,blueMax,blueGamma;
//    
//    CGGetDisplayTransferByFormula(stimulusMonID, &redMin, &redMax, &redGamma, &greenMin, &greenMax, &greenGamma, &blueMin, &blueMax, &blueGamma);
//    mprintf("r:[%f %f %f], g:[%f %f %f], b:[%f %f %f], ",redMin, redMax, redGamma, greenMin, greenMax, greenGamma, blueMin, blueMax, blueGamma);
//
//    redMin     = 0.0f;
//    redMax     = 1.0f;
//    redGamma   = 1.0/1.0f;
//    greenMin   = 0.0f;
//    greenMax   = 1.0f;
//    greenGamma = 1.0/1.0f;
//    blueMin    = 0.0f;
//    blueMax    = 1.0f;
//    blueGamma  = 1.0/1.0f;
//    
//    CGSetDisplayTransferByFormula(stimulusMonID, redMin, redMax, redGamma, greenMin, greenMax, greenGamma, blueMin, blueMax, blueGamma);
//    CGGetDisplayTransferByFormula(stimulusMonID, &redMin, &redMax, &redGamma, &greenMin, &greenMax, &greenGamma, &blueMin, &blueMax, &blueGamma);
//    mprintf("r:[%f %f %f], g:[%f %f %f], b:[%f %f %f], ",redMin, redMax, redGamma, greenMin, greenMax, greenGamma, blueMin, blueMax, blueGamma);
    
    if (loaded)
        return;

    for(int i = 0; i < display->getNContexts(); ++i) {
		
        
        OpenGLContextLock ctxLock = display->setCurrent(i);
        
        GLuint textures[3];
        glGenTextures(3, textures);
        overlay_textures.push_back(textures[0]);
        mask_textures.push_back(textures[1]);
        grating_textures.push_back(textures[2]);
		glDisable(GL_TEXTURE_2D);

		
        // ----------------------------------------
        //                  GRATING
        // ----------------------------------------

        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_1D);
        
		// select our current texture
		glBindTexture( GL_TEXTURE_1D, grating_textures[i] );
		gluBuild1DMipmaps(GL_TEXTURE_1D,
						  GL_LUMINANCE,
						  grating->getSize(),
						  GL_LUMINANCE,
						  GL_FLOAT,
						  grating->get1DData());
        

        // ----------------------------------------
        //                  MASK
        // ----------------------------------------
        
		
		glActiveTextureARB(GL_TEXTURE1_ARB);
        glEnable(GL_TEXTURE_2D);
        
		// select our current texture
		glBindTexture( GL_TEXTURE_2D, mask_textures[i] );
		gluBuild2DMipmaps(GL_TEXTURE_2D,
						  2,
						  mask->getSize(),
						  mask->getSize(),
						  GL_LUMINANCE_ALPHA,
						  GL_FLOAT,
						  mask->get2DData());
		
        // ----------------------------------------
        //                  OVERLAY
        // ----------------------------------------
        
        
        glActiveTextureARB(GL_TEXTURE2_ARB);
        glEnable(GL_TEXTURE_2D);
        
        // select our current texture
        glBindTexture( GL_TEXTURE_2D, overlay_textures[i] );
        gluBuild2DMipmaps(GL_TEXTURE_2D,
                          2,
                          overlay->getSize(),
                          overlay->getSize(),
                          GL_LUMINANCE_ALPHA,
                          GL_FLOAT,
                          overlay->get2DData());
        
        // Unbind grating texture
        glActiveTextureARB(GL_TEXTURE0_ARB); 
        glBindTexture( GL_TEXTURE_1D, 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);
        
        // Unbind overlay texture
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glBindTexture( GL_TEXTURE_1D, 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);
        
        // Unbind mask texture
        glActiveTextureARB(GL_TEXTURE2_ARB);
        glBindTexture( GL_TEXTURE_1D, 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);
        
        glActiveTextureARB(0);

	}
    
    loaded = true;
}   

void WhiteNoiseStimulus::unload(shared_ptr<StimulusDisplay> display) {
    if (!loaded)
        return;

    for (int i = 0; i < display->getNContexts(); i++) {
        OpenGLContextLock ctxLock = display->setCurrent(i);
        glDeleteTextures(1, &(overlay_textures[i]));
        glDeleteTextures(1, &(mask_textures[i]));
        glDeleteTextures(1, &(grating_textures[i]));
    }
    
    overlay_textures.clear();
    mask_textures.clear();
    grating_textures.clear();
    
    loaded = false;
}

void WhiteNoiseStimulus::drawFrame(shared_ptr<StimulusDisplay> display) {
    
    bool invertit = false;

    float mycolor[4];
    mycolor[0]=mycolor[1]=mycolor[2]=0.5f;  // contrast of zero goes to this value

    float con = contrast->getValue().getFloat();
    float opa = opacity->getValue().getFloat();

    // see if contrast is negative (flip the phase)
    if (con<0.0)
    {
        invertit = true;
        con = -1.0f*con;
    }
    
    // see if contrast is out of range, if so, clip at 1.0;
    if (con>1.0) con = 1.0f;
    mycolor[3]=con;
    
    //const std::string &mask_type = mask->getName().c_str();

    // ----------------------------------------
    //                  GRATING
    // ----------------------------------------
    
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);  // Associate with texture 0
    glEnable(GL_TEXTURE_1D);

    // select modulate to mix texture with color for shading
    //glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    // select our current texture
    glBindTexture( GL_TEXTURE_1D, grating_textures[display->getCurrentContextIndex()] );
    
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_1D, GL_TEXTURE_WRAP_T, GL_REPEAT );


    glEnable(GL_TEXTURE_1D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Implement contrast modulation.
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_COMBINE_ARB);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_TEXTURE);
    if (invertit)
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_ONE_MINUS_SRC_COLOR);
    else
        glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_ALPHA);

    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB,  GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB,  GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);

    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,mycolor);

    // ----------------------------------------
    //              MASK
    // ----------------------------------------
    
    glActiveTextureARB(GL_TEXTURE1_ARB); // associate with TEXTURE2_ARB
    glEnable(GL_TEXTURE_2D);
    
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, mask_textures[display->getCurrentContextIndex()] );
    
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create a hybrid texture of Grating + Mask
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    
    // FOR RBG values, replace the current RGB with the previous texture's RGB
    
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    
    // FOR ALPHA values, replace the current ALPHA with the current texture's ALPHA * Opacity value
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_PRIMARY_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, GL_SRC_ALPHA);
    
    //glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,mycolor);
    glColor4f(1,1,1,opa);
    

    // ----------------------------------------
    //              OVERLAY
    // ----------------------------------------
    
    glActiveTextureARB(GL_TEXTURE2_ARB); // associate with TEXTURE1_ARB
    glEnable(GL_TEXTURE_2D);
    
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, overlay_textures[display->getCurrentContextIndex()] );
    
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Create a hybrid texture of Grating + Mask
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE_ARB);
    
    // FOR RBG values, replace the current RGB with the previous texture's RGB
    
    //*
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    // */
    /*
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB, GL_INTERPOLATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB, GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB, GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB, GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB, GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB, GL_SRC_COLOR);
    // */
    // FOR ALPHA values, replace the current ALPHA with the current texture's ALPHA * Opacity value
    //*
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_MODULATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_ALPHA_ARB, GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_ALPHA_ARB, GL_SRC_ALPHA);
    //*/
    /*
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB, GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB, GL_PREVIOUS);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);
    */
    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR,mycolor);
    
    // ----------------------------------------
    //               SETUP COORDINATES
    // ----------------------------------------

    glPushMatrix();
    glTranslatef(xoffset->getValue().getFloat(), yoffset->getValue().getFloat(), 0);
    glRotatef(rotation->getValue().getFloat(),0,0,1);
    GLfloat scale_size = std::max(width->getValue().getFloat(), height->getValue().getFloat());
    glScalef(scale_size, scale_size, 1.0); // scale it up
    
    // ----------------------------------------
    //              DRAW THE QUAD
    // ----------------------------------------
    
    glBegin(GL_QUADS);
    
    
    // here's the description of this equation
    // starting_phase is in degrees ->  degrees*pi/180 = radians
    //
    // speed is in degrees/second
    // spatial_frequency is in cycles/degree
    // elapsed_time is time since the start the 'play' in us
    // us/100000 = seconds
    //
    // degrees   cycles    1 second    pi radians
    // ------- * ------ * ---------- * ----------- * us = radians of phase
    //    s      degree   1000000 us   180 degrees
    //
    // multiply by -1 so it the grating goes in the correct direction
    
    MWTime elapsed_time = getElapsedTime();
    double elapsed_seconds = (double)elapsed_time /  (double)1000000;

    const double aspect = width->getValue().getFloat()/height->getValue().getFloat();

    const double phase   = -1*(  starting_phase->getValue().getFloat()*(M_PI/180.) +   temporal_frequency->getValue().getFloat()*(2.*M_PI)*elapsed_seconds);
    const double o_phase = -1*(o_starting_phase->getValue().getFloat()*(M_PI/180.) + o_temporal_frequency->getValue().getFloat()*(2.*M_PI)*elapsed_seconds);
    
    const double direction_in_radians = direction_in_degrees->getValue().getFloat()*(M_PI/180.);
    const float f          = cos(direction_in_radians);
    const float g          = sin(direction_in_radians);
    const float d          = ((f+g)-1)/2;
    const float texture_bl = 0-d;
    const float texture_br = texture_bl+f;
    const float texture_tr = 1+d;
    const float texture_tl = texture_bl+g;
    const float phase_proportion =    phase/(2*M_PI);
    const float cycle_proportion =    spatial_frequency->getValue().getFloat() * scale_size;
    
    const double overlay_direction_in_radians = 0.0f - (rotation->getValue().getFloat()*(M_PI/180.0f)) + (o_rotation->getValue().getFloat()*(M_PI/180.0f));
    const float f2         = cos(overlay_direction_in_radians);
    const float g2         = sin(overlay_direction_in_radians);
    const float d2         = ((f2+g2)-1)/2;
    const float overlay_bl = 0-d2;
    const float overlay_br = overlay_bl+f2;
    const float overlay_tr = 1+d2;
    const float overlay_tl = overlay_bl+g2;
    const float phase_proportion2 = o_phase/(2*M_PI);
    const float cycle_proportion2 = o_spatial_frequency->getValue().getFloat() * scale_size;
    
    const float mask_s_ratio = 1-std::min(1.0,aspect);
    const float mask_t_ratio = 1-std::min(1.0,1.0/aspect);
    
    
    glNormal3f(0.0, 0.0, 1.0);
    
    glMultiTexCoord1fARB(GL_TEXTURE0_ARB, (cycle_proportion*texture_bl)+phase_proportion);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0-mask_s_ratio, 0-mask_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (cycle_proportion2*overlay_bl)+phase_proportion2,0);
    glVertex3f(-0.5,-0.5,0);
    
    glMultiTexCoord1fARB(GL_TEXTURE0_ARB, (cycle_proportion*texture_br)+phase_proportion);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1+mask_s_ratio, 0-mask_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (cycle_proportion2*overlay_br)+phase_proportion2,0);
    glVertex3f(0.5,-0.5,0);
    
    glMultiTexCoord1fARB(GL_TEXTURE0_ARB, (cycle_proportion*texture_tr)+phase_proportion);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1+mask_s_ratio, 1+mask_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (cycle_proportion2*overlay_tr)+phase_proportion2,0);
    glVertex3f(0.5,0.5,0);
    
    glMultiTexCoord1fARB(GL_TEXTURE0_ARB, (cycle_proportion*texture_tl)+phase_proportion);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0-mask_s_ratio, 1+mask_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE2_ARB, (cycle_proportion2*overlay_tl)+phase_proportion2,0);
    glVertex3f(-0.5,0.5,0);
    
    glEnd(); // GL_QUADS
    
	last_phase = phase*(180/M_PI);
	
	
    // ----------------------------------------
    //                CLEAN-UP
    // ----------------------------------------
    
    // Unbind grating texture
    glActiveTextureARB(GL_TEXTURE0_ARB); 
    glBindTexture( GL_TEXTURE_1D, 0 );
    glDisable(GL_TEXTURE_1D);
    
    // Unbind mask texture
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
	
    // Unbind overlay texture
    glActiveTextureARB(GL_TEXTURE2_ARB);
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);

    glActiveTextureARB(0);
	
	glPopMatrix();

}

Datum WhiteNoiseStimulus::getCurrentAnnounceDrawData() {
	boost::mutex::scoped_lock locker(stim_lock);
	Datum announce_data = StandardDynamicStimulus::getCurrentAnnounceDrawData();

	announce_data.addElement(STIM_TYPE, "drifting_grating");
    announce_data.addElement("grating",              grating->getName());
    announce_data.addElement("overlay",              overlay->getName());
    announce_data.addElement("mask",                 mask->getName());
	
    announce_data.addElement("xoffset",              xoffset->getValue());
	announce_data.addElement("yoffset",              yoffset->getValue());
	announce_data.addElement("width",                width->getValue());
	announce_data.addElement("height",               height->getValue());

    announce_data.addElement("contrast",             contrast->getValue());
    announce_data.addElement("opacity",              opacity->getValue());
    
    announce_data.addElement("rotation",             rotation->getValue());
    announce_data.addElement("direction",            direction_in_degrees->getValue());
    announce_data.addElement("spatial_frequency",    spatial_frequency->getValue());
	announce_data.addElement("temporal_frequency",   temporal_frequency->getValue());
	announce_data.addElement("starting_phase",       starting_phase->getValue());
	
    announce_data.addElement("current_phase",        last_phase);

    announce_data.addElement("o_rotation",           o_rotation->getValue());
    announce_data.addElement("o_direction",          o_direction_in_degrees->getValue());
    announce_data.addElement("o_spatial_frequency",  o_spatial_frequency->getValue());
    announce_data.addElement("o_temporal_frequency", o_temporal_frequency->getValue());
    announce_data.addElement("o_starting_phase",     o_starting_phase->getValue());
    
    announce_data.addElement("o_current_phase",      o_last_phase);

    return announce_data;
}

























