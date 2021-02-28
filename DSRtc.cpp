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
    if(!m_rtc.begin())            // attempt to init the rtc
    {
        Serial.println("Error: could not find RTC");
    }
    Serial.println("init RTC");

    /* debug only (would request this from bt connected phone) */
    m_rtc.adjust(DateTime(2021, 2, 24, 19, 0, 9));    // set date time to 24/02/21 19:00.00
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