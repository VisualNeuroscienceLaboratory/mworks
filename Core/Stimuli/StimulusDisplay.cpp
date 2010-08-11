/**
 * StimulusDisplay.cpp
 *
 * History:
 * Dave Cox on ??/??/?? - Created.
 * Paul Jankunas on 05/23/05 - Fixed spacing.
 *
 * Copyright 2005 MIT.  All rights reserved.
 */

#include "OpenGLContextManager.h"
#include "Stimulus.h" 
#include "StimulusNode.h"
#include "Utilities.h"
#include "StandardVariables.h"
#include "Experiment.h"
#include "StandardVariables.h"
#include "ComponentRegistry.h"
#include "VariableNotification.h"

#include "boost/bind.hpp"


#ifdef	__APPLE__
	#include <AGL/agl.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
    #include <CoreVideo/CVHostTime.h>
#elif	linux
	// TODO: where are these in linux?
#endif

#if M_OPENGL_SHARED_STATE == 1
    //#define RENDER_ONCE
#endif


using namespace mw;



/**********************************************************************
 *                  StimulusDisplay Methods
 **********************************************************************/
StimulusDisplay::StimulusDisplay() {
    current_context_index = -1;

    // defer creation of the display chain until after the stimulus display has been created
    display_stack = shared_ptr< LinkedList<StimulusNode> >(new LinkedList<StimulusNode>());
    
	setDisplayBounds();

    opengl_context_manager = OpenGLContextManager::instance();
    clock = Clock::instance();
    
    needDraw = false;
    
    framebuffer = 0;
    renderbuffer = 0;
    
    if (kCVReturnSuccess != CVDisplayLinkCreateWithActiveCGDisplays(&displayLink)) {
        throw SimpleException("Unable to create display link");
    }
        
    stateSystemNotification = shared_ptr<VariableCallbackNotification>(new VariableCallbackNotification(boost::bind(&StimulusDisplay::stateSystemCallback, this, _1, _2)));
    state_system_mode->addNotification(stateSystemNotification);
}

StimulusDisplay::~StimulusDisplay(){
    stateSystemNotification->remove();
    CVDisplayLinkRelease(displayLink);
    
#ifdef RENDER_ONCE
    if (renderbuffer) {
        glDeleteRenderbuffersEXT(1, &renderbuffer);
    }
    if (framebuffer) {
        glDeleteFramebuffersEXT(1, &framebuffer);
    }
#endif
}

void StimulusDisplay::setCurrent(int i){
    if ((i >= getNContexts()) || (i < 0)) {
        merror(M_DISPLAY_MESSAGE_DOMAIN, "invalid context index (%d)", i);
        return;
    }

	current_context_index = i;
	opengl_context_manager->setCurrent(context_ids[i]); 
}

shared_ptr<StimulusNode> StimulusDisplay::addStimulus(shared_ptr<Stimulus> stim) {
    if(!stim) {
        mprintf("Attempt to load NULL stimulus");
        return shared_ptr<StimulusNode>();
    }

	shared_ptr<StimulusNode> stimnode(new StimulusNode(stim));
	
    display_stack->addToFront(stimnode);
	
	return stimnode;
}

void StimulusDisplay::addStimulusNode(shared_ptr<StimulusNode> stimnode) {
    if(!stimnode) {
        mprintf("Attempt to load NULL stimulus");
        return;
    }
    
	// remove it, in case it already belongs to a list
	stimnode->remove();
	
	display_stack->addToFront(stimnode);  // TODO
}

void StimulusDisplay::setDisplayBounds(){
  shared_ptr<mw::ComponentRegistry> reg = mw::ComponentRegistry::getSharedRegistry();
  shared_ptr<Variable> main_screen_info = reg->getVariable(MAIN_SCREEN_INFO_TAGNAME);
  
 Datum display_info = *main_screen_info; // from standard variables
	if(display_info.getDataType() == M_DICTIONARY &&
	   display_info.hasKey(M_DISPLAY_WIDTH_KEY) &&
	   display_info.hasKey(M_DISPLAY_HEIGHT_KEY) &&
	   display_info.hasKey(M_DISPLAY_DISTANCE_KEY)){
	
		GLdouble width_unknown_units = display_info.getElement(M_DISPLAY_WIDTH_KEY);
		GLdouble height_unknown_units = display_info.getElement(M_DISPLAY_HEIGHT_KEY);
		GLdouble distance_unknown_units = display_info.getElement(M_DISPLAY_DISTANCE_KEY);
	
		GLdouble half_width_deg = (180. / M_PI) * atan((width_unknown_units/2.)/distance_unknown_units);
		GLdouble half_height_deg = half_width_deg * height_unknown_units / width_unknown_units;
		//GLdouble half_height_deg = (180. / M_PI) * atan((height_unknown_units/2.)/distance_unknown_units);
		
		left = -half_width_deg;
		right = half_width_deg;
		top = half_height_deg;
		bottom = -half_height_deg;
	} else {
		left = M_STIMULUS_DISPLAY_LEFT_EDGE;
		right = M_STIMULUS_DISPLAY_RIGHT_EDGE;
		top = M_STIMULUS_DISPLAY_TOP_EDGE;
		bottom = M_STIMULUS_DISPLAY_BOTTOM_EDGE;
	}
	
	mprintf("Display bounds set to (%g left, %g right, %g top, %g bottom)",
			left, right, top, bottom);
}

void StimulusDisplay::getDisplayBounds(GLdouble &left, GLdouble &right, GLdouble &bottom, GLdouble &top) {
    left = this->left;
    right = this->right;
    bottom = this->bottom;
    top = this->top;
}

void StimulusDisplay::addContext(int _context_id){
	context_ids.push_back(_context_id);
	current_context_index = context_ids.size() - 1;
    opengl_context_manager->setCurrent(_context_id);

#ifdef RENDER_ONCE
    if (!(glewIsSupported("GL_EXT_framebuffer_object") && glewIsSupported("GL_EXT_framebuffer_blit"))) {
        throw SimpleException("renderer does not support required OpenGL framebuffer extensions");
    }
    
    if (!framebuffer && (current_context_index > 0)) {
        setCurrent(0);

        getCurrentViewportSize(mainDisplayWidth, mainDisplayHeight);
        
        glGenFramebuffersEXT(1, &framebuffer);
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, framebuffer);
        glGenRenderbuffersEXT(1, &renderbuffer);
        glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, renderbuffer);
        glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_RGBA8, mainDisplayWidth, mainDisplayHeight);
        glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_RENDERBUFFER_EXT, renderbuffer);
        
        GLenum status = glCheckFramebufferStatusEXT(GL_FRAMEBUFFER_EXT);
        if (GL_FRAMEBUFFER_COMPLETE_EXT != status) {
            merror(M_DISPLAY_MESSAGE_DOMAIN, "framebuffer setup failed (status = %d)", status);
            glDeleteRenderbuffersEXT(1, &renderbuffer);
            glDeleteFramebuffersEXT(1, &framebuffer);
            renderbuffer = 0;
            framebuffer = 0;
        }
        
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
    }
#endif
}

void StimulusDisplay::getCurrentViewportSize(GLint &width, GLint &height) {
    GLint viewport[4];
    glGetIntegerv(GL_VIEWPORT, viewport);
    width = viewport[2];
    height = viewport[3];
}


void StimulusDisplay::stateSystemCallback(const Datum &data, MWorksTime time) {
    boost::mutex::scoped_lock lock(display_lock);

    int newState = data.getInteger();

    if (IDLE == newState) {
        
        if (CVDisplayLinkIsRunning(displayLink)) {
            if (kCVReturnSuccess != CVDisplayLinkStop(displayLink)) {
                merror(M_DISPLAY_MESSAGE_DOMAIN, "Unable to stop display updates");
            } else {
                mprintf(M_DISPLAY_MESSAGE_DOMAIN, "Display updates stopped");
            }
        }
        
    } else if (RUNNING == newState) {

        if (!CVDisplayLinkIsRunning(displayLink)) {

            lastFrameTime = 0;

            if (kCVReturnSuccess != CVDisplayLinkSetOutputCallback(displayLink,
                                                                   &StimulusDisplay::displayLinkCallback,
                                                                   this) ||
                kCVReturnSuccess != opengl_context_manager->prepareDisplayLinkForMainDisplay(displayLink) ||
                kCVReturnSuccess != CVDisplayLinkStart(displayLink))
            {
                merror(M_DISPLAY_MESSAGE_DOMAIN, "Unable to schedule display updates");
            } else {
                mprintf(M_DISPLAY_MESSAGE_DOMAIN,
                        "Display updates started (main = %d, current = %d)",
                        CGMainDisplayID(),
                        CVDisplayLinkGetCurrentCGDisplay(displayLink));
            }

        }
        
    }
}


CVReturn StimulusDisplay::displayLinkCallback(CVDisplayLinkRef _displayLink,
                                              const CVTimeStamp *now,
                                              const CVTimeStamp *outputTime,
                                              CVOptionFlags flagsIn,
                                              CVOptionFlags *flagsOut,
                                              void *_display)
{
    StimulusDisplay *display = static_cast<StimulusDisplay*>(_display);
    
    {
        boost::mutex::scoped_lock lock(display->display_lock);
        
//#define WARN_ON_SKIPPED_REFRESH
#ifdef WARN_ON_SKIPPED_REFRESH
        if (display->lastFrameTime) {
            int64_t delta = (outputTime->videoTime - display->lastFrameTime) - outputTime->videoRefreshPeriod;
            if (delta) {
                mwarning(M_DISPLAY_MESSAGE_DOMAIN,
                         "Skipped %g display refresh cycles",
                         (double)delta / (double)(outputTime->videoRefreshPeriod));
            }
        }
#endif
        display->lastFrameTime = outputTime->videoTime;
        
        display->refreshDisplay();
        display->refreshComplete = true;
    }
    
    // Signal waiting threads that refresh is complete
    display->refreshCond.notify_all();
    
    return kCVReturnSuccess;
}


void StimulusDisplay::refreshDisplay() {
    // Just to be safe, make sure we have at least one context
    if (context_ids.size() == 0) {
        return;
    }
    
    //
    // Determine whether we need to draw
    //
    
    if (!needDraw) {
        shared_ptr<StimulusNode> node = display_stack->getFrontmost();
        while (node) {
            needDraw = node->needDraw();
            if (needDraw)
                break;
            node = node->getNext();
        }
    }
    
    if (!needDraw) {
        return;
    }

    //
    // Draw stimuli on main display
    //

    MWTime before_draw = clock->getCurrentTimeUS();

    setCurrent(0);
    drawDisplayStack();
    
#define USE_GL_FENCE
#ifdef USE_GL_FENCE
    if(opengl_context_manager->hasFence()){
        glSetFenceAPPLE(opengl_context_manager->getFence());
    }
#endif
    
    opengl_context_manager->flush(0);
    
#ifdef USE_GL_FENCE
    if(opengl_context_manager->hasFence()){
        glFinishFenceAPPLE(opengl_context_manager->getFence());
    }
#endif
    
    MWTime now = clock->getCurrentTimeUS();
    stimDisplayUpdate->setValue(getAnnounceData(), now);
    announceDisplayStack(now);
    
#define ERROR_ON_LATE_FRAMES
#ifdef ERROR_ON_LATE_FRAMES
    int refresh_rate = opengl_context_manager->getDisplayRefreshRate(opengl_context_manager->getMainDisplayIndex());
    if(refresh_rate <= 0){
        refresh_rate = 60;
    }

    MWTime slop = 2*(1000000/refresh_rate);
    
    if(now-before_draw > slop) {
        merror(M_DISPLAY_MESSAGE_DOMAIN,
               "updating main window display is taking longer than two frames (%lld > %lld) to update", 
               now-before_draw, 
               slop);		
    }
#endif

    
    //
    // Draw stimuli on mirror display(s)
    //

#ifndef RENDER_ONCE
    for (int i = 1; i < context_ids.size(); i++) {
        setCurrent(i);
        drawDisplayStack();
        opengl_context_manager->updateAndFlush(i);
    }	
#else
    if (context_ids.size() > 1) {
        glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, 0);
        glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, framebuffer);
        glBlitFramebufferEXT(0, 0, mainDisplayWidth, mainDisplayHeight,
                             0, 0, mainDisplayWidth, mainDisplayHeight,
                             GL_COLOR_BUFFER_BIT,
                             GL_LINEAR);
        
        glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
        
        for (int i = 1; i < context_ids.size(); i++) {
            setCurrent(i);
            
            GLint dstWidth, dstHeight;
            getCurrentViewportSize(dstWidth, dstHeight);
            
            glBindFramebufferEXT(GL_READ_FRAMEBUFFER_EXT, framebuffer);
            glBindFramebufferEXT(GL_DRAW_FRAMEBUFFER_EXT, 0);
            glBlitFramebufferEXT(0, 0, mainDisplayWidth, mainDisplayHeight,
                                 0, 0, dstWidth, dstHeight,
                                 GL_COLOR_BUFFER_BIT,
                                 GL_LINEAR);
            
            opengl_context_manager->updateAndFlush(i);
        }	
    }
#endif /*RENDER_ONCE*/
    
    needDraw = false;
}


void StimulusDisplay::clearDisplay() {
    {
        boost::mutex::scoped_lock lock(display_lock);

        shared_ptr<StimulusNode> node = display_stack->getFrontmost();
        while(node) {
            node->setVisible(false);
            node = node->getNext();
        }
    }
	
	updateDisplay();
}


void StimulusDisplay::glInit() {

    glShadeModel(GL_FLAT);
    glDisable(GL_BLEND);
    glDisable(GL_DITHER);
    glDisable(GL_FOG);
    glDisable(GL_LIGHTING);
    
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); // DDC added
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity(); // Reset The Projection Matrix
    
    gluOrtho2D(left, right, bottom, top);
    glMatrixMode(GL_MODELVIEW);
    
    glClearColor(0.5, 0.5, 0.5, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

}


void StimulusDisplay::drawDisplayStack() {
    // OpenGL setup

    glInit();
    
    // Draw all of the stimuli in the chain, back to front

    shared_ptr<StimulusNode> node = display_stack->getBackmost();
    while (node) {
        if (node->isVisible()) {
            node->draw(shared_from_this());
        }
        node = node->getPrevious();
    }
}


void StimulusDisplay::updateDisplay() {
	boost::mutex::scoped_lock lock(display_lock);
    
    needDraw = true;
    
    shared_ptr<StimulusNode> node = display_stack->getFrontmost();
    while (node) {
        if (node->isPending()) {
            // we're taking care of the pending state, so
            // clear this flag
            node->clearPending();
            
            // convert "pending visible" stimuli to "visible" ones
            node->setVisible(node->isPendingVisible());
            
            if (node->isPendingRemoval()) {
                node->clearPendingRemoval();
                node->remove();
                continue;
            }
        }
        
        node = node->getNext();
    }

    if (!CVDisplayLinkIsRunning(displayLink)) {
        // Need to do the refresh here
        refreshDisplay();
    } else {
        // Wait for next display refresh to complete
        refreshComplete = false;
        do {
            refreshCond.wait(lock);
        } while (!refreshComplete);
    }
}


void StimulusDisplay::announceDisplayStack(MWTime time) {
    shared_ptr<StimulusNode> node = display_stack->getBackmost(); //tail;
	
    while(node != shared_ptr< LinkedListNode<StimulusNode> >()) {
		if(node->isVisible()) {
            node->announceStimulusDraw(time); 
		}
		
		node = node->getPrevious();
    }
	
}


Datum StimulusDisplay::getAnnounceData() {
    shared_ptr<StimulusNode> node = display_stack->getBackmost(); //tail;
	
    Datum stimAnnounce(M_LIST, 1);
    while(node != shared_ptr< LinkedListNode<StimulusNode> >()) {
		if(node->isVisible()) {
            Datum individualAnnounce(node->getCurrentAnnounceDrawData());
			if(!individualAnnounce.isUndefined()) {
				stimAnnounce.addElement(individualAnnounce);
			}
		}
		
		node = node->getPrevious();
    }	
	return stimAnnounce;
}





