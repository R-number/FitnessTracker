/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "GPS.h"
#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"
#include "StepCount.h"
#include "HeartRate.h"

/* global objects */
DSRtc rtc;
GPS gps(Serial2);
Display display;
StepCount stepCount;
HeartRate heartRate;
BTComms btComms(Serial1, rtc, gps, stepCount, heartRate);
/* ------ */

void setup()
{
    Serial.begin(115200);               // begin serial for debug - this is used in other init modules to give current status
    rtc.init();                         // init the rtc
    btComms.init();                     // init the bt comms
    display.init();                     // init the display
    gps.init();                         // init the GPS
    stepCount.init();                   // init the stepCount
    pinMode(LED_BUILTIN, OUTPUT);       // set the led as an output for debug
}


void loop()
{
    static uint32_t deltaTime = 0;
    static DateTime currentDT;

    if(millis() - deltaTime > 500)          // every 500ms...
    {   /* flash the led to show we're alive */
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        deltaTime = millis();               // update delta time 
        currentDT = rtc.get();              // get the current time

        if(currentDT.hour() == 0 && currentDT.second() == 0)        
        {                           // at midnight...
            stepCount.reset();      // set the step count to 0
            gps.resetDistance();    // set the distance travelled to 0
        }

        /* write to the display, note display will only update if data has changed */
        display.showTime(currentDT);
        display.showGps(gps.isValid(), gps.getDistance());
        display.showSteps(stepCount.get());
        display.showHR(heartRate.get());
    }
    
    /* poll all the modules to keep data updated */
    gps.loop();
    stepCount.loop();
    heartRate.loop();
    btComms.loop();
}

