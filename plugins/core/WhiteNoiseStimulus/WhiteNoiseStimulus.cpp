/*
 *  WhiteNoiseStimulus.cpp
 *  MWorksCore
 *
 *  Created by romesh kumbhani on 2015-10-07.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#include "WhiteNoiseStimulus.h"

#include "EllipseMask.h"
#include "FlattopMask.h"
#include "GaussianMask.h"
#include "RectangleMask.h"
#include "SinusoidMask.h"

#include <MWorksCore/StandardStimuli.h>

#include <algorithm>



const std::string WhiteNoiseStimulus::NOISE("noise");
const std::string WhiteNoiseStimulus::MASK("mask");

const std::string WhiteNoiseStimulus::OPACITY("opacity");

const std::string WhiteNoiseStimulus::NCHECKS_X("n_cols");
const std::string WhiteNoiseStimulus::NCHECKS_Y("n_rows");

const std::string WhiteNoiseStimulus::DISTRIB_MU("distrib_mu");
const std::string WhiteNoiseStimulus::DISTRIB_RANGE("distrib_range");
const std::string WhiteNoiseStimulus::NFRAMES("nframes");
const std::string WhiteNoiseStimulus::SEED("seed");

const std::string WhiteNoiseStimulus::MEAN("mean");
const std::string WhiteNoiseStimulus::STD_DEV("std_dev");



void WhiteNoiseStimulus::describeComponent(ComponentInfo &info) {
    StandardDynamicStimulus::describeComponent(info);
    
    info.setSignature("stimulus/white_noise");

    info.addParameter(BasicTransformStimulus::X_SIZE,     "1.0");
    info.addParameter(BasicTransformStimulus::Y_SIZE,     "1.0");
    
    info.addParameter(NOISE,                              "ternary");
    info.addParameter(MASK,                               "rectangle");
    info.addParameter(BasicTransformStimulus::X_POSITION, "0.0");
    info.addParameter(BasicTransformStimulus::Y_POSITION, "0.0");
    
    info.addParameter(BasicTransformStimulus::ROTATION,   "0.0");
    info.addParameter(OPACITY,                            "1.0");

    info.addParameter(NCHECKS_X,                          "16.0");
    info.addParameter(NCHECKS_Y,                          "16.0");

    info.addParameter(DISTRIB_MU,                         "0.0");
    info.addParameter(DISTRIB_RANGE,                      "1.0");
    info.addParameter(NFRAMES,                            "3");
    info.addParameter(SEED,                               "1");
    
    info.addParameter(STD_DEV,                            "1.0");
    info.addParameter(MEAN,                               "0.0");
 
}


WhiteNoiseStimulus::WhiteNoiseStimulus(const ParameterValueMap &parameters) :
    StandardDynamicStimulus(parameters),
    width(registerVariable(parameters[BasicTransformStimulus::X_SIZE])),
    height(registerVariable(parameters[BasicTransformStimulus::Y_SIZE])),
    x_position(registerVariable(parameters[BasicTransformStimulus::X_POSITION])),
    y_position(registerVariable(parameters[BasicTransformStimulus::Y_POSITION])),
    rotation(parameters[BasicTransformStimulus::ROTATION]),
    opacity(registerVariable(parameters[OPACITY])),
    nchecks_x(registerVariable(parameters[NCHECKS_X])),
    nchecks_y(registerVariable(parameters[NCHECKS_Y])),
    distrib_mu(registerVariable(parameters[DISTRIB_MU])),
    distrib_range(registerVariable(parameters[DISTRIB_RANGE])),
    nframes(registerVariable(parameters[NFRAMES])),
    seed(parameters[SEED])
{
    mprintf("Making WhiteNoiseStimulus object.");
    noise_str = parameters[NOISE].str();
    mask_str  = parameters[MASK].str();
    shared_ptr<Variable> mask_size(new ConstantVariable(256L));
    
    shared_ptr<mw::ComponentRegistry> reg = mw::ComponentRegistry::getSharedRegistry();
    shared_ptr<Variable> main_screen_info = reg->getVariable(MAIN_SCREEN_INFO_TAGNAME);
    Datum display_info = *main_screen_info; // from standard variables
    float mon_width_arb = (float)display_info.getElement(M_DISPLAY_WIDTH_KEY);
    float mon_dist_arb  = (float)display_info.getElement(M_DISPLAY_DISTANCE_KEY);
    
    float mon_width_deg  = atan2f(mon_width_arb, mon_dist_arb)*180.0/M_PI;
    CGDirectDisplayID activeDisplays[6];
    uint32_t displayCount;
    CGGetActiveDisplayList(6, activeDisplays, &displayCount);
    stimMonID = activeDisplays[display_info.getElement(M_DISPLAY_TO_USE_KEY)];
    size_t mon_width_pix = CGDisplayPixelsWide (stimMonID);
    pixperdeg = mon_width_pix / mon_width_deg;
    degperpix = mon_width_deg / mon_width_pix;
    
    float width_pix  = width->getValue().getFloat() * pixperdeg;
    float height_pix = height->getValue().getFloat() * pixperdeg;
    
    nsubchecks_x = std::max(1u,(uint)round(width_pix/nchecks_x->getValue().getFloat()));
    nsubchecks_y = std::max(1u,(uint)round(height_pix/nchecks_y->getValue().getFloat()));
    
    width_deg  =(float)nsubchecks_x * (float)nchecks_x->getValue().getInteger() * degperpix;
    height_deg =(float)nsubchecks_y * (float)nchecks_y->getValue().getInteger() * degperpix;
    
    mprintf("checksize: %d x %d pix | gridsize: %lld x %lld elements | fieldsize: %.2f x %.2f deg",
            nsubchecks_x,nsubchecks_y,nchecks_x->getValue().getInteger(),nchecks_y->getValue().getInteger(), width_deg, height_deg);
    
    // --------------
    noise = shared_ptr<NoiseData>(new NoiseData(noise_str, nsubchecks_x, nsubchecks_y,
                                                nchecks_x->getValue().getInteger(), nchecks_y->getValue().getInteger(),
                                                seed, distrib_mu->getValue().getFloat(), distrib_range->getValue().getFloat()));

	if (mask_str == "rectangle") {
		mask = shared_ptr<Mask>(new RectangleMask(mask_size));
	} else if (mask_str == "ellipse") {
		mask = shared_ptr<Mask>(new EllipseMask(mask_size));
    } else if (mask_str == "flattop") {
        mask = shared_ptr<Mask>(new FlattopMask(mask_size,8));
    } else if (mask_str == "flattop8") {
        mask = shared_ptr<Mask>(new FlattopMask(mask_size,8));
    } else if (mask_str == "flattop4") {
        mask = shared_ptr<Mask>(new FlattopMask(mask_size,4));
	} else if (mask_str == "gaussian") {
		mask = shared_ptr<Mask>(new GaussianMask(mask_size, VariablePtr(parameters[MEAN]), VariablePtr(parameters[STD_DEV])));
    } else if (mask_str == "sinusoid") {
        mask = shared_ptr<Mask>(new SinusoidMask(mask_size));
	} else {
        throw SimpleException("illegal mask", mask_str);
	}

}


void WhiteNoiseStimulus::load(shared_ptr<StimulusDisplay> display) {
    mprintf("WhiteNoiseStimulus:Load()");
    if (loaded)
        return;

    for(int i = 0; i < display->getNContexts(); ++i) {
        OpenGLContextLock ctxLock = display->setCurrent(i);
        GLuint textures[2];
        glGenTextures(2, textures);
        mask_textures.push_back(textures[0]);  // mask_textures gets the first GLTexture
        noise_textures.push_back(textures[1]); // noise_textures gets the second GLTexture
		glDisable(GL_TEXTURE_2D);

        // ----------------------------------------
        //                  NOISE
        // ----------------------------------------

        glActiveTextureARB(GL_TEXTURE0_ARB);
        glEnable(GL_TEXTURE_2D);
        
        // select our current texture
        glBindTexture(GL_TEXTURE_2D, noise_textures[i]);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, noise->getSizeW(), noise->getSizeH(), 0, GL_LUMINANCE_ALPHA, GL_FLOAT, noise->getData());
        
        // Unbind noise texture
        glActiveTextureARB(GL_TEXTURE0_ARB);
        glBindTexture( GL_TEXTURE_1D, 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);

        // ----------------------------------------
        //                  MASK
        // ----------------------------------------
        
        
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glEnable(GL_TEXTURE_2D);
        
        // select our current texture
        glBindTexture(GL_TEXTURE_2D, mask_textures[i] );
        gluBuild2DMipmaps(GL_TEXTURE_2D,
                          2,
                          mask->getSize(),
                          mask->getSize(),
                          GL_LUMINANCE_ALPHA,
                          GL_FLOAT,
                          mask->getData());

        // Unbind mask texture
        glActiveTextureARB(GL_TEXTURE1_ARB);
        glBindTexture( GL_TEXTURE_1D, 0 );
        glBindTexture( GL_TEXTURE_2D, 0 );
        glDisable(GL_TEXTURE_1D);
        glDisable(GL_TEXTURE_2D);

        glActiveTextureARB(0);
	}
    
    loaded = true;
}   


void WhiteNoiseStimulus::unload(shared_ptr<StimulusDisplay> display) {
    mprintf("WhiteNoiseStimulus:Unload()");
    if (!loaded)
        return;

    for (int i = 0; i < display->getNContexts(); i++) {
        OpenGLContextLock ctxLock = display->setCurrent(i);
        glDeleteTextures(1, &(mask_textures[i]));
        glDeleteTextures(1, &(noise_textures[i]));
    }
    
    mask_textures.clear();
    noise_textures.clear();
    
    loaded = false;
}


void WhiteNoiseStimulus::drawFrame(shared_ptr<StimulusDisplay> display) {
    
    float mycolor[4];
    mycolor[0]=mycolor[1]=mycolor[2]=0.5f;  // contrast of zero goes to this value

    float con = 1.0f;
    float opa = opacity->getValue().getFloat();

    mycolor[3]=con;

    // ----------------------------------------
    //                  NOISE
    // ----------------------------------------
    
    glDisable(GL_TEXTURE_2D);
    glActiveTextureARB(GL_TEXTURE0_ARB);  // Associate with texture 0
    glEnable(GL_TEXTURE_2D);

    // select modulate to mix texture with color for shading
    //glTexEnvi( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMBINE );
    
    // select our current texture
    glBindTexture( GL_TEXTURE_2D, noise_textures[display->getCurrentContextIndex()] );
    if (curframe<((uint)nframes->getValue().getInteger()))
        curframe++;
    else{
        noise->genNewData();
        glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE_ALPHA, noise->getSizeW(), noise->getSizeH(), 0, GL_LUMINANCE_ALPHA, GL_FLOAT, noise->getData());
        curframe = 1;
    }
    
    // when texture area is small, bilinear filter the closest mipmap
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    //glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE );


    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    // Implement contrast modulation.
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE,   GL_COMBINE_ARB);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_RGB_ARB,    GL_INTERPOLATE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_RGB_ARB,    GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_RGB_ARB,   GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE1_RGB_ARB,    GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND1_RGB_ARB,   GL_SRC_COLOR);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE2_RGB_ARB,    GL_CONSTANT);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND2_RGB_ARB,   GL_SRC_ALPHA);

    glTexEnvi(GL_TEXTURE_ENV, GL_COMBINE_ALPHA_ARB,  GL_REPLACE);
    glTexEnvi(GL_TEXTURE_ENV, GL_SOURCE0_ALPHA_ARB,  GL_TEXTURE);
    glTexEnvi(GL_TEXTURE_ENV, GL_OPERAND0_ALPHA_ARB, GL_SRC_ALPHA);

    glTexEnvfv(GL_TEXTURE_ENV, GL_TEXTURE_ENV_COLOR, mycolor);

    // ----------------------------------------
    //              MASK
    // ----------------------------------------
    
    glActiveTextureARB(GL_TEXTURE1_ARB); // associate with TEXTURE1_ARB
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
    
    // Create a hybrid texture of Noise + Mask
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
    //               SETUP COORDINATES
    // ----------------------------------------

    glPushMatrix();
    glTranslatef(x_position->getValue().getFloat(), y_position->getValue().getFloat(), 0);
    glRotatef(rotation->getValue().getFloat(),0,0,1);
    
    //GLfloat scale_size = std::max(width_deg, height_deg);
    //GLfloat scale_size = std::max(width->getValue().getFloat(), height->getValue().getFloat());
    //glScalef(scale_size, scale_size, 1.0); // scale it up
    glScalef((GLfloat)width_deg, (GLfloat)height_deg, 1.0); // scale it up
    // ----------------------------------------
    //              DRAW THE QUAD
    // ----------------------------------------
    
    glBegin(GL_QUADS);
    
    glNormal3f(0.0, 0.0, 1.0);
    
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
    // multiply by -1 so it the noise goes in the correct direction
    
    //elapsed_time = getElapsedTime();
    //mprintf("%08.3f ms",(double)elapsed_time /(double)1000);
    
    //const double aspect = width->getValue().getFloat()/height->getValue().getFloat();
    /*
    const double aspect = width_deg/height_deg;

    const float noise_s_ratio = 1-std::min(1.0,aspect);
    const float noise_t_ratio = 1-std::min(1.0,1.0/aspect);
    const float mask_s_ratio  = 1-std::min(1.0,aspect);
    const float mask_t_ratio  = 1-std::min(1.0,1.0/aspect);
    
    
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0-noise_s_ratio, 0-noise_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0-mask_s_ratio,  0-mask_t_ratio);
    glVertex3f(-0.5,-0.5,0);
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1+noise_s_ratio, 0-noise_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1+mask_s_ratio,  0-mask_t_ratio);
    glVertex3f(0.5,-0.5,0);
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1+noise_s_ratio, 1+noise_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1+mask_s_ratio,  1+mask_t_ratio);
    glVertex3f(0.5,0.5,0);
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0-noise_s_ratio, 1+noise_t_ratio);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0-mask_s_ratio,  1+mask_t_ratio);
    glVertex3f(-0.5,0.5,0);
    */
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 0);
    glVertex3f(-0.5,-0.5,0);
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 0);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 0);
    glVertex3f(0.5,-0.5,0);
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 1, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 1, 1);
    glVertex3f(0.5,0.5,0);
    
    glMultiTexCoord2fARB(GL_TEXTURE0_ARB, 0, 1);
    glMultiTexCoord2fARB(GL_TEXTURE1_ARB, 0, 1);
    glVertex3f(-0.5,0.5,0);
    
    
    
    
    glEnd(); // GL_QUADS
    
    // ----------------------------------------
    //                CLEAN-UP
    // ----------------------------------------
    
    // Unbind noise texture
    glActiveTextureARB(GL_TEXTURE0_ARB); 
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
    
    // Unbind mask texture
    glActiveTextureARB(GL_TEXTURE1_ARB);
    glBindTexture( GL_TEXTURE_2D, 0 );
    glDisable(GL_TEXTURE_2D);
	
    glActiveTextureARB(0);
	
	glPopMatrix();

}


Datum WhiteNoiseStimulus::getCurrentAnnounceDrawData() {
	boost::mutex::scoped_lock locker(stim_lock);
	Datum announce_data = StandardDynamicStimulus::getCurrentAnnounceDrawData();

	announce_data.addElement(STIM_TYPE,              "white_noise");
    announce_data.addElement("noise",                noise->getName());
    announce_data.addElement("mask",                 mask->getName());
	
    announce_data.addElement("x_position",           x_position->getValue());
	announce_data.addElement("y_position",           y_position->getValue());
	announce_data.addElement("width",                width->getValue());
	announce_data.addElement("height",               height->getValue());

    announce_data.addElement("rotation",             rotation->getValue());
    announce_data.addElement("opacity",              opacity->getValue());

    
    announce_data.addElement("nsubchecks_x",         Datum(&nsubchecks_x));
    announce_data.addElement("nsubchecks_y",         Datum(&nsubchecks_y));

    announce_data.addElement("nchecks_x",            nchecks_x->getValue());
    announce_data.addElement("nchecks_y",            nchecks_y->getValue());

    announce_data.addElement("distrib_mu",           distrib_mu->getValue());
    announce_data.addElement("distrib_range",        distrib_range->getValue());

    announce_data.addElement("nframes",              nframes->getValue());
    announce_data.addElement("seed",                 seed->getValue());
    
    GLfloat *tmp = (GLfloat *)noise->getBlockData();
    Datum noiseData(reinterpret_cast<char *>(tmp), noise->getSizeBT() * sizeof(GLfloat));
    announce_data.addElement("noise_vals",   noiseData);

    mprintf("%.2f,%.2f,%.2f,%.2f",tmp[0],tmp[1],tmp[2],tmp[3]);
    mprintf("%d,%d",nsubchecks_x,nsubchecks_y);
    return announce_data;
}
