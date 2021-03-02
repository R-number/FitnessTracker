/*
Authour:        Team R-Number
Date Created:   01/03/2021
File name:      GPS.cpp
*/

#include "GPS.h"

#define GPS_PORT_NAME   "Serial2"

GPS::GPS(HardwareSerial &GPSSerialPort)
    :m_gpsPort(GPSSerialPort)
{
    /* empty constructor */
}

void GPS::init()
{
    Serial.println("init GPS");
    Serial.print( F("NMEA.INO: started\n") );
    Serial.print( F("  fix object size = ") );
    Serial.println( sizeof(m_gpsParser.fix()) );
    Serial.print( F("  gps object size = ") );
    Serial.println( sizeof(m_gpsParser) );
    Serial.println( F("Looking for GPS device on " GPS_PORT_NAME) );

    #ifndef NMEAGPS_RECOGNIZE_ALL
    #error You must define NMEAGPS_RECOGNIZE_ALL in NMEAGPS_cfg.h!
    #endif

    #ifdef NMEAGPS_INTERRUPT_PROCESSING
    #error You must *NOT* define NMEAGPS_INTERRUPT_PROCESSING in NMEAGPS_cfg.h!
    #endif

    #if !defined( NMEAGPS_PARSE_GGA ) & !defined( NMEAGPS_PARSE_GLL ) & \
        !defined( NMEAGPS_PARSE_GSA ) & !defined( NMEAGPS_PARSE_GSV ) & \
        !defined( NMEAGPS_PARSE_RMC ) & !defined( NMEAGPS_PARSE_VTG ) & \
        !defined( NMEAGPS_PARSE_ZDA ) & !defined( NMEAGPS_PARSE_GST )

    Serial.println( F("\nWARNING: No NMEA sentences are enabled: no fix data will be displayed.") );

    #else
    if (m_gpsParser.merging == NMEAGPS::NO_MERGING) {
        Serial.print  ( F("\nWARNING: displaying data from ") );
        Serial.print  ( m_gpsParser.string_for( LAST_SENTENCE_IN_INTERVAL ) );
        Serial.print  ( F(" sentences ONLY, and only if ") );
        Serial.print  ( m_gpsParser.string_for( LAST_SENTENCE_IN_INTERVAL ) );
        Serial.println( F(" is enabled.\n"
                            "  Other sentences may be parsed, but their data will not be displayed.") );
    }
    #endif

    Serial.print  ( F("\nGPS quiet time is assumed to begin after a ") );
    Serial.print  ( m_gpsParser.string_for( LAST_SENTENCE_IN_INTERVAL ) );
    Serial.println( F(" sentence is received.\n"
                        "  You should confirm this with NMEAorder.ino\n") );
                        
    m_gpsPort.begin(9600);
}

void GPS::monitor()
{

    // while(m_gpsPort.available() > 0)
    // {
    //     Serial.write(m_gpsPort.read());
    // }

    while(m_gpsParser.available(m_gpsPort))
    {
        m_fix = m_gpsParser.read();

        Serial.print("Location: ");
        if (m_fix.valid.location) 
        {
            Serial.print( m_fix.latitude(), 6 );
            Serial.print( ',' );
            Serial.print( m_fix.longitude(), 6 );
        }

        Serial.print(", Altitude: ");
        if (m_fix.valid.altitude)
        {
            Serial.print( m_fix.altitude() );
        }

        Serial.println();
    }
}