/**
* Authour:      Team R-Number
* Date Created: 24/02/21
*/

#include <RTClib.h>

RTC_DS3231 rtc;     // setup the rtc object

void setup()
{
    Serial.begin(115200);       // begin serial for debug

    if(!rtc.begin())            // attempt to init the rtc
    {
        Serial.println("Error: could not find RTC");
    }

    /* debug only (would request this from bt connected phone) */
    rtc.adjust(DateTime(2021, 2, 24, 19, 0, 9));    // set date time to 24/02/21 19:00.00
    
}

void loop()
{
    

}

void printTime()
{
    /* code taken from ds3231.ino example and adapted into printTime function */
     DateTime currentTime = rtc.now();
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