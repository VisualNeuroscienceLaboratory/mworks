//
//  PR650.cpp
//  PR650
//
//  Created by VNL stimulus on 8/21/15.
//  Copyright (c) 2015 New York University - Visual Neuroscience Lab. All rights reserved.
//

#include "PR650DriverDevice.h"

BEGIN_NAMESPACE_MW_PR650_DRIVER

const std::string Device::SYNC_FREQUENCY("sync_frequency");
const std::string Device::INTEGRATION_TIME("integration_time");
const std::string Device::AVG_CNT("avg_cnt");
const std::string Device::UNIT_TYPE("unit_type");

void Device::describeComponent(ComponentInfo &info) {
    IODevice::describeComponent(info);
    
    info.setSignature("iodevice/pr650_driver");
    
    info.addParameter(SYNC_FREQUENCY,   "0");
    info.addParameter(INTEGRATION_TIME, "0");
    info.addParameter(AVG_CNT,          "1");
    info.addParameter(UNIT_TYPE,        "1");
}


Device::Device(const ParameterValueMap &parameters) :
IODevice(parameters),
sync_frequency(parameters[SYNC_FREQUENCY]),
integration_time(parameters[INTEGRATION_TIME]),
avg_cnt(parameters[AVG_CNT]),
unit_type(parameters[UNIT_TYPE])
{
    // stuff to do when object is created
    mprintf("PR650 Constructor");
}


Device::~Device() {
    lock_guard lock(ObjectLock);
    // stuff to do when object is destroyed
    mprintf("PR650 Destructor");
    closeSerialPort();

}


bool Device::initialize() {
    lock_guard lock(ObjectLock);
    // stuff to do when IO device is initialized
    mprintf("PR650 Initialize");
    return true;

}


bool Device::startDeviceIO() {
    lock_guard lock(ObjectLock);
    // stuff to do when IO device is started (connect to PR650)
    if (started){
        mprintf("PR650 Start I/O : Already started!");
    }
    started = false;
    
    if (!bsdPath[0]) {
        mprintf("No modem port found. Check the header!\n");
        return false;
    }
    
    char tmpbuff[read_size];
    bool portopened = false;
    mprintf("Waiting for PR650...(10 tries)");
    for (unsigned int tries=0;tries<3;tries++){
        fileDescriptor = openSerialPort(bsdPath);
        if (-1 == fileDescriptor)
            break;
        mprintf("opened: %d",fileDescriptor);
        WriteToTTY("M0",true);
        sleep(1);
        ReadFromTTY(tmpbuff,true);
        if (tmpbuff[0]==0){
            closeSerialPort();
            //usleep(1000000); // wait 1 s
            mprintf(".");
        }
        else
        {
            portopened=true;
            break;
        }
    }
    
    if (!portopened)
    {
        started = false;
        return false;
    }
    mprintf("Setting up the serial port.\n");

    // setup port
    
    char tmp[30];
    
    unsigned int sfreq    = (unsigned int)sync_frequency->getValue().getInteger();
    unsigned int inttime  = (unsigned int)integration_time->getValue().getInteger();
    unsigned int avgcnt   = (unsigned int)avg_cnt->getValue().getInteger();
    unsigned int unittype = (unsigned int)unit_type->getValue().getInteger();
    
    if ((sfreq<40)&&(sfreq != 0))
    {
        mprintf("Sync Frequency too low (<40), using 40.");
        sfreq = 40;
    }
    if (sfreq>250)
    {
        mprintf("Sync Frequency too high (>250), using 250.");
        sfreq = 250;
    }
    
    if ((inttime<10)&&(inttime != 0))
    {
        mprintf("Integration time too low (<10 ms), using 10.");
        inttime = 10;
    }
    if (inttime>6000)
    {
        mprintf("Integration time too high (>6000 ms), using 6000.");
        inttime = 6000;
    }
    
    if (avgcnt<1)
    {
        mprintf("Integration time too low (<1 measurement), using 1.");
        avgcnt = 1;
    }
    if (avgcnt>6000)
    {
        mprintf("Integration time too high (>99 measurements), using 99.");
        avgcnt = 99;
    }

    if (unittype>1)
    {
        mprintf("Unit type can only be 0 or 1. Using 0 (SI Units, cd/m^2)");
        unittype = 0;
    }

    if (sfreq>0)
        sprintf(tmp,"S01,,,,%d,%d,%d,%d",sfreq,inttime,avgcnt,unittype);
    else
        sprintf(tmp,"S01,,,,,%d,%d,%d",inttime,avgcnt,unittype);
    
    WriteToTTY(tmp,false);
    ReadFromTTY(tmpbuff,false);
    SetBrightness(3);
    
    mprintf("PR650 Start I/O : Started.");
    return true;
}


bool Device::stopDeviceIO() {
    lock_guard lock(ObjectLock);
    // stuff to do with IO device is disconnected (disconnect from PR650)
    if (!started){
        mprintf("PR650 Stop I/O : PR650 wasn't started. Nothing to stop.");
        return true;
    }
    mprintf("PR650 Stop I/O : Stopped.");
    closeSerialPort();
    started = false;
    return true;
}

void Device::GetLuminance(double value){
    lock_guard lock(ObjectLock);
    if (!started)
        return;
    mprintf("PR650 GetLuminance");
    // stuff to do when getting the current luminance
    
    
    
    value = 0.0;
}



int Device::openSerialPort(const char * bsdPath)
{
    int             fD = -1;
    int             handshake;
    struct termios  options;
    
    fD = open(bsdPath, O_RDWR | O_NOCTTY | O_NONBLOCK);
    if (fD == -1) {
        merror(M_IODEVICE_MESSAGE_DOMAIN,"Error opening serial port %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    if (ioctl(fD, TIOCEXCL) == -1) {
        mprintf("Error setting TIOCEXCL on %s - %s(%d).\n",bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    if (fcntl(fD, F_SETFL, 0) == -1) {
        mprintf("Error clearing O_NONBLOCK %s - %s(%d).\n",bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    if (tcgetattr(fD, &gOriginalTTYAttrs) == -1) {
        mprintf("Error getting tty attributes %s - %s(%d).\n",bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    options = gOriginalTTYAttrs;
    cfmakeraw(&options);
    options.c_cc[VMIN]  = 0;
    options.c_cc[VTIME] = 10;
    
    cfsetspeed(&options, B9600);       // Set 9600 baud
    options.c_cflag |= (CS8        |    // Use 8 bit words
                        ~PARENB    );    // Parity enable (even parity if PARODD not also set)
    
    options.c_iflag |= IGNCR;
    
    speed_t speed = 9600; // Set 9600 baud
    if (ioctl(fD, IOSSIOSPEED, &speed) == -1) {
        mprintf("Error calling ioctl(..., IOSSIOSPEED, ...) %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
    }
    
    if (tcsetattr(fD, TCSANOW, &options) == -1) {
        mprintf("Error setting tty attributes %s - %s(%d).\n",
               bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    if (ioctl(fD, TIOCSDTR) == -1) mprintf("Error asserting DTR %s - %s(%d).\n",bsdPath, strerror(errno), errno);
    if (ioctl(fD, TIOCCDTR) == -1) mprintf("Error clearing DTR %s - %s(%d).\n", bsdPath, strerror(errno), errno);
    handshake = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
    if (ioctl(fD, TIOCMSET, &handshake) == -1) mprintf("Error setting handshake lines %s - %s(%d).\n",bsdPath, strerror(errno), errno);
    if (ioctl(fD, TIOCMGET, &handshake) == -1) mprintf("Error getting handshake lines %s - %s(%d).\n",bsdPath, strerror(errno), errno);
    unsigned long mics = 1UL;        // set latency to 1 microsecond
    
    if (ioctl(fD, IOSSDATALAT, &mics) == -1) {
        mprintf("Error setting read latency %s - %s(%d).\n",bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    // Success
    return fD;
    
    // Failure path
}

void Device::closeSerialPort()
{
    if (tcdrain(fileDescriptor) == -1) mprintf("Error waiting for drain - %s(%d).\n",strerror(errno), errno);
    close(fileDescriptor);
}

void Device::WriteToTTY(const char * msg,const bool echoout)
{
    char tmp[30];
    sprintf(tmp,"%s\n",msg);
    if (echoout) mprintf("> %s",tmp);
    write(fileDescriptor, (void *)tmp, (strlen(tmp))*sizeof(char));
    //usleep(500000); // wait 1/2 s
}

void Device::ReadFromTTY(char * buf,const bool echoout)
{
    char        buffer[read_size];   // Input buffer1
    char *      bufPtr;        // Current char in buffer
    ssize_t     numBytes;       // Number of bytes read or written
    
    // Read characters into our buffer until we get a CR or LF
    bufPtr = buffer;  // points to first element
    do {
        numBytes = read(fileDescriptor, bufPtr, &buffer[sizeof(buffer)] - bufPtr - 1);
        
        if (numBytes == -1) {
            mprintf("Error reading from modem - %s(%d).\n", strerror(errno), errno);
        }
        else if (numBytes > 0)
        {
            bufPtr += numBytes;
            if (*(bufPtr - 1) == '\n' || *(bufPtr - 1) == '\r') {
                break;
            }
        }
    } while (numBytes > 0);
    
    // NUL terminate the string and see if we got an OK response
    *bufPtr = '\0';
    
    //mprintf("< %s\n", logString(buffer));
    if (echoout) mprintf("< %s", buffer);
    strcpy(buf,buffer);
}

bool Device::SetBrightness(int level)
{
    char tmpbuff[read_size];
    switch (level) {
        case 0:
            WriteToTTY("B0",false);
            break;
        case 1:
            WriteToTTY("B1",false);
            break;
        case 2:
            WriteToTTY("B2",false);
            break;
        case 3:
            WriteToTTY("B3",false);
            break;
        default:
            WriteToTTY("B0",false);
            break;
    }
    ReadFromTTY(tmpbuff,false);
    if (tmpbuff[0] == 0)
        return false;
    else
        return true;
}

END_NAMESPACE_MW_PR650_DRIVER

