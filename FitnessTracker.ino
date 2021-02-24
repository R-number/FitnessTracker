/**
* Authour:      Team R-Number
* Date Created: 24/02/21
*/

#include "rtc.h"

DSRtc rtc;

void setup()
{
    Serial.begin(115200);       // begin serial for debug
    rtc.init();                 // init the rtc object
}

void loop()
{
    rtc.printTime();            // print out the current time

}