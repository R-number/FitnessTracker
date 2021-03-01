/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"
#include "GPS.h"

DSRtc rtc;
#if defined(ARDUINO_SAM_DUE)
BTComms btComms(Serial1);       // setup the bluetooth with Serial port 1 for Arduino Due
GPS gps(Serial2);
#else
BTComms btComms(Serial);        // for other arduino's use the only serial port
GPS gps(Serial);
#endif
Display display;

#define HEARTRATE_PIN   A0


void setup()
{
    Serial.begin(115200);               // begin serial for debug - this is used in other init functions to give current status
    rtc.init();                         // init the rtc
    btComms.init();                     // init the bt comms
    display.init();                     // init the display - note this also runs multiple test functions
    gps.init();
}

void loop()
{
    DateTime now = rtc.get();
    display.showTime(now);
    display.monitorTimeout(now, 5*60);      // timeout after 5 mins

    gps.monitor();


    delay(500);
    btComms.read();
    btComms.write();
    // display.increment();

    // Serial.print("Heartrate val: ");    // read heartrate and print to console
    // Serial.println(analogRead(HEARTRATE_PIN));
}