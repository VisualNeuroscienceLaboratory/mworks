/*
 *  WhiteNoiseUtilities.cpp
 *  WhiteNoiseStimulusPlugin
 *
 *  Created by bkennedy on 11/13/08.
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "WhiteNoiseUtilities.h"

unsigned int WhiteNoiseUtilities::getNextPowerOfTwo(unsigned int number) {
	number--;
	for (int i=1; i<sizeof(unsigned int)*8; i=i*2) {
		number = number | number >> i;
	}
	++number;
	
	return number;
}


