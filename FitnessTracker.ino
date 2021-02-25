/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"

DSRtc rtc;
BTComms btComms(Serial1);       // setup the bluetooth with Serial port 1
Display display;

#define HEARTRATE_PIN   A0


void setup()
{
    Serial.begin(115200);               // begin serial for debug - this is used in other init functions to give current status
    rtc.init();                         // init the rtc
    btComms.init();                     // init the bt comms
    display.init();                     // init the display - note this also runs multiple test functions
}

void loop()
{
    rtc.printTime();                    // print out the current time
    
    btComms.read();
    btComms.write();

    Serial.print("Heartrate val: ");    // read heartrate and print to console
    Serial.println(analogRead(HEARTRATE_PIN));
}