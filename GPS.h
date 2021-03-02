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
    void loop();
    bool isValid() const { return m_locValid; }

    float getAltitude() const { return m_fix.latitude(); }
    float getLongitude() const { return m_fix.longitude(); }
    float getLatitude() const { return m_fix.latitude(); }
    float getDistance() const { return m_dist; }

private:
    void calculateDistance();

private:
    HardwareSerial &m_gpsPort;
    NMEAGPS m_gpsParser;
    gps_fix m_fix;
    NeoGPS::Location_t m_currentLoc, m_oldLoc;      // used to calculate distance travelled
    float m_dist;
    bool m_locValid;
};