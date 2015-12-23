/*
 *  Utilities.cpp
 *  StimulusPlugin
 *
 *  Created by bkennedy on 11/13/08.
 *  Copyright 2008 mit. All rights reserved.
 *
 */

#include "Utilities.h"

unsigned int Utilities::getNextPowerOfTwo(unsigned int number) {
	number--;
	for (int i=1; i<sizeof(unsigned int)*8; i=i*2) {
		number = number | number >> i;
	}
	++number;
	
	return number;
}


