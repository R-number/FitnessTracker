/*
Authour:        Team R-Number
Date Created:   01/03/2021
File name:      GPS.cpp
*/

#include "GPS.h"

GPS::GPS(HardwareSerial &GPSSerialPort)
    :m_GPSSerial(GPSSerialPort)
{
    /* empty constructor */
}