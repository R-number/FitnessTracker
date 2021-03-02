/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "GPS.h"
#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"

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
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
    static uint32_t deltaTime = 0;

    if(millis() - deltaTime > 500)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        deltaTime = millis();
        DateTime now = rtc.get();
        display.showTime(now);
        display.monitorTimeout(now, 1*60);      // timeout after n mins
    }

    gps.monitor();


    // btComms.read();
    // btComms.write();
    // display.increment();

    // Serial.print("Heartrate val: ");    // read heartrate and print to console
    // Serial.println(analogRead(HEARTRATE_PIN));
}