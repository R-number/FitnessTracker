/*
Authour:        Team R-Number
Date Created:   01/03/2021
File name:      GPS.h
*/

#pragma once

#include <Arduino.h>
#include <NMEAGPS.h>

class GPS
{
public:
    GPS(HardwareSerial &GPSSerialPort);

    void init();
    void monitor();

private:
    HardwareSerial &m_gpsPort;
    NMEAGPS m_gpsParser;
    gps_fix m_fix;
};