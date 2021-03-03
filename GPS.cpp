/*
Authour:        Team R-Number
Date Created:   01/03/2021
File name:      GPS.cpp
*/

#include "GPS.h"

#define GPS_PORT_NAME   "Serial2"
// #define DEBUG_SERIAL

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

    m_dist = 0;
    m_locValid = false;
}

void GPS::loop()
{
    while(m_gpsParser.available(m_gpsPort))     // check if we have data incoming
    {
        m_fix = m_gpsParser.read();             // read in the latest fix

        #ifdef DEBUG_SERIAL
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
        #endif

        calculateDistance();                // calculate the distance when we get a new fix
    }
}


void GPS::calculateDistance()
{
    static uint16_t count = 0;
    static int32_t latAccumulator = 0, lonAccumulator = 0;
    const uint16_t n_samples = 15;

    /* 
    * this function samples valid points to create an average postion
    * this is required as otherwise we will accumulate distance when
    * static due to gps jitter
    */

    if(m_fix.valid.location && m_fix.valid.date && m_fix.valid.time)        // do we have a valid fix?
    {
        latAccumulator += m_fix.location.lat();
        lonAccumulator += m_fix.location.lon();
        count ++;

        if(count >= n_samples)                  // when we have completed sampling
        {
            #ifdef DEBUG_SERIAL
            Serial.print("Accum(lat,long): ");
            Serial.print(latAccumulator);
            Serial.print(", ");
            Serial.println(lonAccumulator);
            #endif

            latAccumulator /= n_samples;        // average the data
            lonAccumulator /= n_samples;
            m_currentLoc.lat(latAccumulator);   // load averaged data into current location
            m_currentLoc.lon(lonAccumulator);
            latAccumulator = 0;                 // reset for next time
            lonAccumulator = 0;

            #ifdef DEBUG_SERIAL
            Serial.print("Avg(lat,long): ");
            Serial.print(m_currentLoc.lat());
            Serial.print(", ");
            Serial.println(m_currentLoc.lon());
            Serial.print("Dist(delta): ");
            Serial.print(m_currentLoc.DistanceKm(m_oldLoc), 10);
            #endif

            /* only accumulate the distance when location is valid and we are above certain speed */
            if(m_locValid && (m_fix.speed_kph() > 0.5f))          
            {
                m_dist += m_currentLoc.DistanceKm(m_oldLoc);    // calculate distance between old and new

                #ifdef DEBUG_SERIAL
                Serial.print("Dist(km): ");
                Serial.println(m_dist, 6);
                #endif
            }
            m_oldLoc = m_currentLoc;                            // old = new
            count = 0;
            m_locValid = true;                                  // we have been round once (or more) - location is valid
        }
    }
    else
    {
        #ifdef DEBUG_SERIAL
        Serial.println("No GPS");
        #endif

        m_locValid = false;
        count = 0;
    }
}