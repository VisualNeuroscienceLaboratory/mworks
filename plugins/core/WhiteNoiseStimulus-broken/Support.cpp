/*
 *  Support.cpp
 *  Support Functions
 *
 *  Created by Romesh Kumbhani on 10/6/15
 *  Copyright 2015 nyu. All rights reserved.
 *
 */

#include "Support.h"

unsigned int Support::getNextPowerOfTwo(unsigned int number) {
	number--;
	for (int i=1; i<sizeof(unsigned int)*8; i=i*2) {
		number = number | number >> i;
	}
	++number;
	
	return number;
}


