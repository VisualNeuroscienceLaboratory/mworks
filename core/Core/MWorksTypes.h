/**
 * MWorksTypes.h
 *
 * Description: Core typedefs
 *
 * History:
 * David Cox on 6/25/05 - Created.
 * Paul Jankunas on 8/5/05 - Added NetworkStats type for connection stats.
 *
 * Copyright 2005 MIT. All rights reserved.
 */

#ifndef MONKEYWORKS_TYPES_H_
#define MONKEYWORKS_TYPES_H_


#include "MWorksMacros.h"


BEGIN_NAMESPACE_MW


// event time
typedef	long long MWTime;
typedef MWTime  MWorksTime;  // crutch for compatibility, for now
        
// value of network statistics
typedef unsigned long long NetworkStats;


END_NAMESPACE_MW


#endif