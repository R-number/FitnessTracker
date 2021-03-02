/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    DSRtc.cpp
*/

#include "DSRtc.h"

DSRtc::DSRtc()
{
    /* empty constructor */
}

void DSRtc::init()
{
    static uint8_t count = 0;
    while(!m_rtc.begin() && count < 50)            // attempt to init the rtc
    {
        count ++;
        Serial.println("Error: could not find RTC");
        delay(10);
    }
    Serial.println("init RTC");

    /* debug only (would request this from bt connected phone) */
    m_rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));    // set date time to compile date & time
    print(get());
}

const DateTime& DSRtc::get() const
{
    return m_rtc.now();
}

void DSRtc::set(DateTime &t)
{
    m_rtc.adjust(t);
}

void DSRtc::print(const DateTime &t)
{
    Serial.print(t.day(), DEC);
    Serial.print('/');
    Serial.print(t.month(), DEC);
    Serial.print('/');
    Serial.print(t.year(), DEC);
    Serial.print(" ");
    Serial.print(t.hour(), DEC);
    Serial.print(':');
    Serial.print(t.minute(), DEC);
    Serial.print(':');
    Serial.print(t.second(), DEC);
    Serial.println();
}