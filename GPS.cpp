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

void GPS::init()
{
    m_GPSSerial.begin(9600);
    Serial.println("init GPS");
}

void GPS::monitor()
{
    while(m_GPSSerial.available() > 0)
    {
        Serial.write(m_GPSSerial.read());
    }
}