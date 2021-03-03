/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"
#include <DFRobot_Heartrate.h>

DSRtc rtc;
#if defined(ARDUINO_SAM_DUE)
BTComms btComms(Serial1);       // setup the bluetooth with Serial port 1 for Arduino Due
#else
BTComms btComms(Serial);        // for other arduino's use the only serial port
#endif
Display display;
DFRobot_Heartrate heartrate(ANALOG_MODE); ///< ANALOG_MODE or DIGITAL_MODE

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
    // DateTime now = rtc.get();
    // display.showTime(now);

    // delay(500);
    // btComms.read();
    // btComms.write();
    // display.increment();

    uint8_t rateValue;
    heartrate.getValue(HEARTRATE_PIN);
    rateValue = heartrate.getRate();
    if(rateValue)
    {
        Serial.println(rateValue);
    }
    delay(20);

}