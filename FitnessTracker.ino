/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "GPS.h"
#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"
#include "DFRobot_Heartrate.h"

DSRtc rtc;
#if defined(ARDUINO_SAM_DUE)
BTComms btComms(Serial1);       // setup the bluetooth with Serial port 1 for Arduino Due
GPS gps(Serial2);
#else
BTComms btComms(Serial);        // for other arduino's use the only serial port
GPS gps(Serial);
#endif
Display display;
DFRobot_Heartrate heartSense(DIGITAL_MODE);
#define HEARTRATE_PIN   A0
#define BUFFER_SAMPLES   200


uint8_t StabiliseRate()
{
    uint8_t rateBuffer=0, idle=0 ,ThrownRate=0;
    heartSense.getValue(HEARTRATE_PIN);
    uint16_t cumulativeRate=0;    
    while(rateBuffer<BUFFER_SAMPLES)
    {
        uint8_t bpm =heartSense.getRate();
        if (bpm)
        {
            Serial.print(rateBuffer);
            Serial.print(" BPM: ");
            Serial.println(bpm);
            idle=0;
            cumulativeRate+=bpm;            
        }
        else
        {
            idle++;
            ThrownRate++;
        } 
        rateBuffer++;
        delay(20);
    } 
    if (idle<80)
    {
        return cumulativeRate/(BUFFER_SAMPLES-ThrownRate);
    }
    else
        return 0;


    if(millis() - deltaTime > 20)
    {
        heartSense.getValue(HEARTRATE_PIN);
        uint8_t bpm = heartSense.getRate();

        if(bpm)
        {
            cumulativeRate += bpm;
            count++;
        }
        else
        {
            cumulativeRate = 0;
        }

        if(count >= 200)
        {
            cumulativeRate /= count;
        }
    }

    return cumulativeRate;
}

void setup()
{
    Serial.begin(9600);               // begin serial for debug - this is used in other init functions to give current status
    //rtc.init();                         // init the rtc
    btComms.init();                     // init the bt comms
    display.init();                     // init the display - note this also runs multiple test functions
    gps.init();
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

    }
    
    gps.loop();     // poll the gps


    btComms.read();
    btComms.write();
    // display.increment();

    // Serial.print("Heartrate val: ");    // read heartrate and print to console
    // Serial.println(analogRead(HEARTRATE_PIN));
    //DateTime now = rtc.get();
    //heartSense.getValue(HEARTRATE_PIN);
   // uint16_t val = analogRead(HEARTRATE_PIN);
    uint8_t bpmValue = StabiliseRate();
    //Serial.print("Analogue: ");
    //Serial.println(val);
    if(bpmValue)
    {
        Serial.print("Overall BPM: ");
        Serial.println(bpmValue);
    }
    else
    {
        Serial.println("Sensor has no signal");
    }
   
   // display.showTime(now);
    
   //btComms.read();
   //btComms.write();
}