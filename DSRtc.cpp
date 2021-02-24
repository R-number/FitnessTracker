/**
* Authour:      Team R-Number
* Date Created: 24/02/21
*/

#include "rtc.h"

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

    /* debug only (would request this from bt connected phone) */
    m_rtc.adjust(DateTime(2021, 2, 24, 19, 0, 9));    // set date time to 24/02/21 19:00.00
}

void DSRtc::printTime()
{
    /* code taken from ds3231.ino example and adapted into printTime function */
     DateTime currentTime = m_rtc.now();
     char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

    Serial.print(currentTime.year(), DEC);
    Serial.print('/');
    Serial.print(currentTime.month(), DEC);
    Serial.print('/');
    Serial.print(currentTime.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[currentTime.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(currentTime.hour(), DEC);
    Serial.print(':');
    Serial.print(currentTime.minute(), DEC);
    Serial.print(':');
    Serial.print(currentTime.second(), DEC);
    Serial.println();
}