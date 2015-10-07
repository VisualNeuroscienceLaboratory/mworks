/*
 *  GammaStimulusFactory.h
 *  GammaStimulus
 *
 *  Created by Romesh Kumbhani on 07/21/2015.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#ifndef ___PROJECTNAMEASIDENTIFIER____FACTORY_H
#define ___PROJECTNAMEASIDENTIFIER____FACTORY_H

#include <MWorksCore/ComponentFactory.h>

using namespace mw;


class ___PROJECTNAMEASIDENTIFIER___Factory : public ComponentFactory {
	virtual shared_ptr<mw::Component> createObject(std::map<std::string, std::string> parameters,
                                                   ComponentRegistry *reg);
};


#endif
