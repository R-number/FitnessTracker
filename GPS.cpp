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
    char buffer[64];
    uint8_t count = 0;

    if(m_GPSSerial.available())
    {
        while(m_GPSSerial.available())
        {
            buffer[count++] = m_GPSSerial.read();
            if(count == 64)
            {
                break;
            }
        }
        Serial.write(buffer, count);
        memset(buffer, 0, count);
        count = 0;
    }
}