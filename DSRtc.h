/**
* Authour:      Team R-Number
* Date Created: 24/02/21
*/

#pragma once

#include <RTClib.h>

/* class to abstarct use of the DS3231 rtc using RTClib */

class DSRtc
{
public:
    DSRtc();

    void init();
    void printTime();

private:
    RTC_DS3231 m_rtc;     // setup the rtc object
    
};