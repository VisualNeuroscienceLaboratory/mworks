/*
 *  �PROJECTNAME�Factory.cpp
 *  �PROJECTNAME�
 *
 *  Created by �FULLUSERNAME� on �DATE�.
 *  Copyright �YEAR� �ORGANIZATIONNAME�. All rights reserved.
 *
 */

#include "�PROJECTNAME�Factory.h"

#include <boost/regex.hpp>
#include "MWorksCore/ComponentRegistry.h"

using namespace mw;

shared_ptr<mw::Component> �PROJECTNAME�Factory::createObject(std::map<std::string, std::string> parameters,
                                                               mw::ComponentRegistry *reg) {
	REQUIRE_ATTRIBUTES(parameters, 
					   "tag",
                       "another_attribute");
	
	std::string tagname(parameters["tag"]);

    
	shared_ptr<Variable> another_attribute = reg->getVariable(parameters["another_attribute"]);	
	
	shared_ptr <�PROJECTNAME�> new_component = shared_ptr<�PROJECTNAME�>(new �PROJECTNAME�(tagname, another_attribute));
	
	return new_component;
}
