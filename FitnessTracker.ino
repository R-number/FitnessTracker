/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "DSRtc.h"
#include "BTComms.h"

DSRtc rtc;
BTComms btComms(Serial1);       // setup the bluetooth with Serial port 1

void setup()
{
    Serial.begin(115200);       // begin serial for debug
    rtc.init();                 // init the rtc
    btComms.init();             // init the bt comms
}

void loop()
{
    rtc.printTime();            // print out the current time
    
    btComms.read();
    btComms.write();

}