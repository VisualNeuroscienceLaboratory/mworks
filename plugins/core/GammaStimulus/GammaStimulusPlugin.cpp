/*
 *  GammaStimulusPlugins.cpp
 *  GammaStimulusPlugins
 *
 *  Created by Romesh Kumbhani on 08/06/15.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardStimulusFactory.h>

#include "GammaStimulus.h"

using namespace mw;


class GammaStimulusPlugin : public Plugin {
    void registerComponents(shared_ptr<ComponentRegistry> registry) {
        registry->registerFactory<StandardStimulusFactory, GammaStimulus>();
    }
};


MW_SYMBOL_PUBLIC
extern "C" Plugin* getPlugin(){
    return new GammaStimulusPlugin();
}
