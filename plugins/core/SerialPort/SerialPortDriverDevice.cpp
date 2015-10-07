//
//  SerialPort.cpp
//  SerialPort
//
//  Created by VNL stimulus on 8/21/15.
//  Copyright (c) 2015 New York University - Visual Neuroscience Lab. All rights reserved.
//

#include "SerialPortDriverDevice.h"

BEGIN_NAMESPACE_MW_SerialPort_DRIVER

const std::string Device::BAUD      = "baud";
const std::string Device::BITS      = "bits";
const std::string Device::PARITY    = "parity";
const std::string Device::STOP_BIT  = "stop_bit";

void Device::describeComponent(ComponentInfo &info)
{
    IODevice::describeComponent(info);
    
    info.setSignature("iodevice/serialport_driver");
    
    info.addParameter(BAUD,      "9600");
    info.addParameter(BITS,      "8");
    info.addParameter(PARITY,    "none");
    info.addParameter(STOP_BIT,  "1");
}

Device::Device(const ParameterValueMap &parameters) : IODevice( parameters)
{
    // stuff to do when object is created
    mprintf("SerialPort Constructor");

    baud     = parameters[BAUD].as<int>();
    bits     = parameters[BITS].as<int>();
    stop_bit = parameters[STOP_BIT].as<int>();
    parity   = parameters[PARITY].str();

    // baud can be 9600,19200,57600,115200
    if (baud<B19200)                           baud = B9600;
    else if ((baud >= B19200)&&(baud<B38400))  baud = B19200;
    else if ((baud >= B57600)&&(baud<B115200)) baud = B57600;
    else baud = B115200;
 
    // bits can be 6, 7, 8
    if      (bits < 6) bits = 6;
    else if (bits > 8) bits = 8;
    
    if      (stop_bit < 1) stop_bit = 1;
    else if (stop_bit > 2) stop_bit = 2;
    
    switch (tolower(parity.front()))
    {
        case 'n': parity = "none"; break;
        case 'e': parity = "even"; break;
        case 'o': parity = "odd";  break;
        default:  parity = "none";
    }

    mprintf("SerialPort Constructor : Input Setting: %d baud, %d/%s/%d",baud,bits,parity.c_str(),stop_bit);
    
}

Device::~Device()
{
    lock_guard lock(ObjectLock);
    // stuff to do when object is destroyed
    mprintf("SerialPort Destructor");
    closeSerialPort();

}

bool Device::initialize()
{
    lock_guard lock(ObjectLock);
    // stuff to do when IO device is initialized
    mprintf("SerialPort Initialize");
    return true;

}

bool Device::startDeviceIO()
{
    lock_guard lock(ObjectLock);
    mprintf("SerialPort Start I/O");
    // stuff to do when IO device is started (connect to SerialPort)
    if (started){
        mprintf("SerialPort Start I/O : Already started!");
    }
    started = false;
    
    if (!bsdPath[0]) {
        mprintf("SerialPort Start I/O : No modem port found. Check the header!\n");
        return false;
    }
    
    //char tmpbuff[READ_SIZE];
    bool portopened = false;
    mprintf("SerialPort Start I/O : Setting up the serial port : %s.",bsdPath);
    fileDescriptor = openSerialPort();
    mprintf("SerialPort Start I/O : Serial Port Opened.");

    portopened = (-1 != fileDescriptor);

    if (!portopened)
    {
        started = false;
        return false;
    }
    else
        started = true;

    WriteToTTY("Hello\n",true);
    //ReadFromTTY(tmpbuff,false);
    
    mprintf("SerialPort Start I/O : Started.");
    return true;
}

bool Device::stopDeviceIO()
{
    lock_guard lock(ObjectLock);
    mprintf("SerialPort Stop I/O");
    // stuff to do with IO device is disconnected (disconnect from SerialPort)
    if (!started){
        mprintf("SerialPort Stop I/O : SerialPort wasn't started. Nothing to stop.");
        return true;
    }
    mprintf("SerialPort Stop I/O : Stopped.");
    closeSerialPort();
    started = false;
    return true;
}

int Device::openSerialPort()
{
    int             fD = -1;
    int             hs;
    struct termios  options;
    
    unsigned int _baud      = baud;
    unsigned int _bits      = bits;
    std::string  _parity    = parity;
    unsigned int _stop_bit  = stop_bit;
    
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
    options.c_cc[VMIN]  = 1;
    options.c_cc[VTIME] = 10;
    
    if ((_baud >= B9600)&&(_baud<B19200))        _baud = B9600;
    else if ((_baud >= B19200)&&(_baud<B38400))  _baud = B19200;
    else if ((_baud >= B57600)&&(_baud<B115200)) _baud = B57600;
    else _baud = B115200;

    cfsetspeed(&options,_baud);       // Set baud rate to above
    
    
    switch (_bits)
    {
        case 6:  options.c_cflag |= CS6; break;
        case 7:  options.c_cflag |= CS7; break;
        case 8:  options.c_cflag |= CS8; break;
        default: options.c_cflag |= CS8; break;
    }
    
    switch (tolower(_parity.front()))
    {
        case 'n': options.c_cflag |= ~PARENB; break;
        case 'e': options.c_cflag |= (PARENB); break;
        case 'o': options.c_cflag |= (PARENB|PARODD); break;
        default:  options.c_cflag |= ~PARENB; break;
    }
    
    switch (_stop_bit)
    {
        case 1:  options.c_cflag |= ~CSTOPB; break;
        case 2:  options.c_cflag |=  CSTOPB; break;
        default: options.c_cflag |= ~CSTOPB; break;
    }
    
    options.c_cflag |= CLOCAL;
    options.c_cflag |= CREAD;
    
    options.c_iflag |= IGNCR;
    
//    speed_t speed;
    
//    if (ioctl(fD, IOSSIOSPEED, &speed) == -1) {
//        mprintf("Error calling ioctl(..., IOSSIOSPEED, ...) %s - %s(%d).\n",
//                bsdPath, strerror(errno), errno);
//    }
    
    if (tcsetattr(fD, TCSANOW, &options) == -1) {
        mprintf("Error setting tty attributes %s - %s(%d).\n",bsdPath, strerror(errno), errno);
        if (fD != -1) close(fD);
        return -1;
    }
    
    if (ioctl(fD, TIOCSDTR) == -1) mprintf("Error asserting DTR %s - %s(%d).\n",bsdPath, strerror(errno), errno);
    if (ioctl(fD, TIOCCDTR) == -1) mprintf("Error clearing DTR %s - %s(%d).\n", bsdPath, strerror(errno), errno);
    
    hs = TIOCM_DTR | TIOCM_RTS | TIOCM_CTS | TIOCM_DSR;
    if (ioctl(fD, TIOCMSET, &hs) == -1) mprintf("Error setting handshake lines %s - %s(%d).\n",bsdPath, strerror(errno), errno);
    if (ioctl(fD, TIOCMGET, &hs) == -1) mprintf("Error getting handshake lines %s - %s(%d).\n",bsdPath, strerror(errno), errno);
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
    //if (tcdrain(fileDescriptor) == -1) mprintf("Error waiting for drain - %s(%d).\n",strerror(errno), errno);
    close(fileDescriptor);
}

void Device::WriteToTTY(const std::string &msg,const bool echoout)
{
    char c = 'q';
    std::regex ex_NL ("\n");
    if (echoout)
    {
        mprintf("[%d] %s [%lu chars]",fileDescriptor,std::regex_replace(msg,ex_NL,"\\n").c_str(),msg.length());
    }
    write(fileDescriptor, (void *)&c,1);
    //write(fileDescriptor, (void *)msg.c_str(), msg.length()*sizeof(char));
    mprintf("done");

}

void Device::ReadFromTTY(char * buf,const bool echoout)
{
    char        buffer[READ_SIZE];   // Input buffer1
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

END_NAMESPACE_MW_SerialPort_DRIVER

