/*
 *  WhiteNoiseStimulusPlugins.cpp
 *  WhiteNoiseStimulusPlugins
 *
 *  Created by romesh kumbhani on 2015-10-07.
 *  Copyright 2015 NYU. All rights reserved.
 *
 */

#include <MWorksCore/Plugin.h>
#include <MWorksCore/StandardStimulusFactory.h>

#include "WhiteNoiseStimulus.h"

using namespace mw;


class WhiteNoiseStimulusPlugin : public Plugin {
    void registerComponents(shared_ptr<ComponentRegistry> registry) {
        registry->registerFactory<StandardStimulusFactory, WhiteNoiseStimulus>();
    }
};


MW_SYMBOL_PUBLIC
extern "C" Plugin* getPlugin(){
    return new WhiteNoiseStimulusPlugin();
}
