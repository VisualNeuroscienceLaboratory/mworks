//
//  measurement_data.cpp
//  PR650
//
//  Created by VNL stimulus on 8/24/15.
//  Copyright (c) 2015 The MWorks Project. All rights reserved.
//

#include "measurement_data.h"

measurement_data::measurement_data(){
    
    responsestr = "";
    
    quality_code = 0;
    units        = 0;
    luminance    = 0.0;
    chrom_x      = 0.0;
    chrom_y      = 0.0;
    
}


measurement_data::measurement_data(string r){

    responsestr = r;
    
    quality_code = get_quality_code();
    units        = get_units();
    luminance    = get_luminance();
    chrom_x      = get_chrom_x();
    chrom_y      = get_chrom_y();
    
}

