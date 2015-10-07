//
//  measurement_data.h
//  PR650
//
//  Created by VNL stimulus on 8/24/15.
//  Copyright (c) 2015 The MWorks Project. All rights reserved.
//

#ifndef __PR650__measurement_data__
#define __PR650__measurement_data__

#include <stdio.h>
#include <string.h>
#include <math.h>


class measurement_data {
private:
    char   get_quality_code(){ return   (char)(std::stoi(responsestr.substr(0,1)));};
    char   get_units(){        return   (char)(std::stoi(responsestr.substr(3,3)));};
    double get_luminance(){    return (double)(std::stod(responsestr.substr(5,9)) * pow(10.0,(std::stod(responsestr.substr(11,13)))));};
    float  get_chrom_x(){      return  (float)(std::stof(responsestr.substr(15,19)));};
    float  get_chrom_y(){      return  (float)(std::stof(responsestr.substr(21,25)));};
    
public:
    string responsestr;    // response string
    char   quality_code;   // 0 to 30
    char   units;          // 0=luminance, 1=illuminance, 2=uncalibrated
    double luminance;      // 1931 CIE Y
    float  chrom_x;        // 1931 CIE x
    float  chrom_y;        // 1931 CIE y
    measurement_data();
    measurement_data(string responsestr);
    ~measurement_data();
};

#endif /* defined(__PR650__measurement_data__) */



