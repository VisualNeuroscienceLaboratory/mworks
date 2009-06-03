/*
 *  �PROJECTNAME�.h
 *  MWPlugin
 *
 *  Created by David Cox on 2/26/09.
 *  Copyright 2009 Harvard University. All rights reserved.
 *
 */
#ifndef �PROJECTNAME�_PLUGIN_H_
#define �PROJECTNAME�_PLUGIN_H_

#include <MonkeyWorksCore/Plugin.h>
using namespace mw;

extern "C"{
    Plugin *getPlugin();
}

class �PROJECTNAME� : public Plugin {
    
	virtual void registerComponents(shared_ptr<mw::ComponentRegistry> registry);	
};


#endif