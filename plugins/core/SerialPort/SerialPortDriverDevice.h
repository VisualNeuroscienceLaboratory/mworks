//
//  SerialPort.h
//  SerialPort
//
//  Created by VNL stimulus on 8/21/15.
//  Copyright (c) 2015 New York University - Visual Neuroscience Lab. All rights reserved.
//

#ifndef __SerialPortDriver__SerialPortDriverDevice__
#define __SerialPortDriver__SerialPortDriverDevice__

//#include "SerialPortDriverCommand.h"


#include <stdio.h>
#include <string.h>
#include <regex>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sysexits.h>
#include <termios.h>
#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/serial/IOSerialKeys.h>
#include <IOKit/serial/ioss.h>

#define READ_SIZE 1024

#include <CoreFoundation/CoreFoundation.h>
#include <IOKit/serial/IOSerialKeys.h>


BEGIN_NAMESPACE_MW_SerialPort_DRIVER

class Device : public IODevice, boost::noncopyable {

private:
    int         baud;
    int         bits;
    std::string parity;
    int         stop_bit;
    
    std::mutex ObjectLock;
    using lock_guard = std::lock_guard<std::mutex>;
    bool started = false;
    
    int fileDescriptor;
    const char * bsdPath = "/dev/tty.USA49W23P1.1";
    struct termios gOriginalTTYAttrs;
    
    // open serial port
    int openSerialPort();
    void closeSerialPort();
    void WriteToTTY(const std::string &msg, const bool echoout);
    void ReadFromTTY(char * buf, const bool echoout);
    
public:
    //static const std::string DEV;
    static const std::string BAUD;
    static const std::string BITS;
    static const std::string PARITY;
    static const std::string STOP_BIT;
    //static const std::string HANDSHAKE;
    
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
};

END_NAMESPACE_MW_SerialPort_DRIVER


#endif /* defined(__SerialPortDriver__SerialPortDriverDevice__) */


