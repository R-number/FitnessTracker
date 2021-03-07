/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    BTComms.h
*/

#pragma once

#include "GPS.h"
#include "StepCount.h"
#include "HeartRate.h"
#include "DSRtc.h"

#include "Arduino.h"

typedef union
{
    uint32_t VAL;
    struct
    {
        unsigned NIBBLE0 : 4;
        unsigned NIBBLE1 : 4;
        unsigned NIBBLE2 : 4;
        unsigned NIBBLE3 : 4;
        unsigned NIBBLE4 : 4;
        unsigned NIBBLE5 : 4;
        unsigned NIBBLE6 : 4;
        unsigned NIBBLE7 : 4;
    };
} nibbles32_t;

typedef union
{
    uint16_t VAL;
    struct
    {
        unsigned NIBBLE0 : 4;
        unsigned NIBBLE1 : 4;
        unsigned NIBBLE2 : 4;
        unsigned NIBBLE3 : 4;
    };
} nibbles16_t;



class BTComms
{
public:
    BTComms(HardwareSerial &BTSerialPort, DSRtc &rtcRef, 
            GPS &gpsRef, StepCount &scRef, HeartRate &hrRef);

    void init();
    void status();
    void read();
    void write();

private:
    void parse();
    char hexToAscii(uint8_t d);
    uint8_t asciiToHex(char c);

private:
    HardwareSerial &m_BT;       // keep a reference to the hardware serial port we are using
                                // this way we can refer to it without using confusing Serial'n' name
    char m_rxBuf[1024];
    uint16_t m_rxLen;
    /* the following are references to the other module to allow bt
    to set and get data from them */
    DSRtc &m_rtcRef;
    GPS &m_gpsRef;
    StepCount &m_scRef;
    HeartRate &m_hrRef;
};

