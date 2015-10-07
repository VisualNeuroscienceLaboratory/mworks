//
//  PR650.h
//  PR650
//
//  Created by VNL stimulus on 8/21/15.
//  Copyright (c) 2015 New York University - Visual Neuroscience Lab. All rights reserved.
//

#ifndef __PR650Driver__PR650DriverDevice__
#define __PR650Driver__PR650DriverDevice__

#include "PR650DriverCommand.h"
#include "measurement_data.h"


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sysexits.h>
#include <termios.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>

#define MATCH_PATH "/dev/cu.USA49Wfa13P1.1"
#define read_size 1024

//#include <CoreFoundation/CoreFoundation.h>
//#include <IOKit/serial/IOSerialKeys.h>


BEGIN_NAMESPACE_MW_PR650_DRIVER

class Device : public IODevice, boost::noncopyable {
    
public:
    static const std::string SYNC_FREQUENCY;
    static const std::string INTEGRATION_TIME;
    static const std::string AVG_CNT;
    static const std::string UNIT_TYPE;
    
    static void describeComponent(ComponentInfo &info);
    
    // basic functions of IO Devices
    
    // Constructor
    explicit Device(const ParameterValueMap &parameters);
    // Destructor
    ~Device();
    // Init
    bool initialize() override;
    // on use of Start Device I/O
    bool startDeviceIO() override;
    // on use of Stop Device I/O
    bool stopDeviceIO() override;

    // ACTIONS ----------------------------
    void GetLuminance(double value);
    
private:
    shared_ptr<Variable> sync_frequency;
    shared_ptr<Variable> integration_time;
    shared_ptr<Variable> avg_cnt;
    shared_ptr<Variable> unit_type;
    
    
    std::mutex ObjectLock;
    using lock_guard = std::lock_guard<std::mutex>;
    bool started = false;
    
    int fileDescriptor;
    const char * bsdPath = "/dev/cu.USA49W23P1.1";
    struct termios gOriginalTTYAttrs;
    measurement_data * data = new measurement_data();


    // open serial port
    int  openSerialPort(const char * bsdPath);
    void closeSerialPort();
    void WriteToTTY(const char * msg, const bool echoout);
    void ReadFromTTY(char * buf, const bool echoout);
    bool SetBrightness(int level);

};

END_NAMESPACE_MW_PR650_DRIVER


#endif /* defined(__PR650Driver__PR650DriverDevice__) */


