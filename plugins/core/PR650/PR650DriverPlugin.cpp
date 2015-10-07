//
//  PR650DriverPlugin.cpp
//  PR650Driver
//
//  Created by VNL stimulus on 8/21/15.
//  Copyright (c) 2015 New York University - Visual Neuroscience Lab. All rights reserved.
//

#include "PR650DriverDevice.h"


BEGIN_NAMESPACE_MW_PR650_DRIVER


class Plugin : public mw::Plugin {
    void registerComponents(boost::shared_ptr<ComponentRegistry> registry) override {
        registry->registerFactory<StandardComponentFactory, Device>();
    }
};


extern "C" mw::Plugin* getPlugin() {
    return new Plugin();
}


END_NAMESPACE_MW_PR650_DRIVER