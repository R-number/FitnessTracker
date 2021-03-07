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

DSRtc rtc;
GPS gps(Serial2);
Display display;
StepCount stepCount;
HeartRate heartRate;

BTComms btComms(Serial1, rtc, gps, stepCount, heartRate);


void setup()
{
    Serial.begin(115200);               // begin serial for debug - this is used in other init functions to give current status
    rtc.init();                         // init the rtc
    btComms.init();                     // init the bt comms
    display.init();                     // init the display - note this also runs multiple test functions
    gps.init();
    stepCount.init();
    pinMode(LED_BUILTIN, OUTPUT);
}


void loop()
{
    static uint32_t deltaTime = 0;
    static DateTime currentDT;

    if(millis() - deltaTime > 500)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        deltaTime = millis();
        currentDT = rtc.get();
        display.showTime(currentDT);
        // display.monitorTimeout(currentDT, 2*60);      // timeout after n mins
        if(gps.isValid())
        {
            display.showGpsData(gps.getLatitude(), gps.getLongitude(), gps.getAltitude(), gps.getDistance());
        }
        display.showGpsSignal(gps.isValid());
        display.showSteps(stepCount.get());
        display.showHR(heartRate.get());
    }
    
    gps.loop();         // poll the gps
    stepCount.loop();
    heartRate.loop();

    btComms.read();
    // btComms.write();
}

