/*
Authour:        Team R-Number
Date Created:   01/03/2021
File name:      GPS.h
*/

#pragma once

class GPS
{
public:
    GPS(HardwareSerial &GPSSerialPort);

private:
    HardwareSerial &m_GPSSerial;
};