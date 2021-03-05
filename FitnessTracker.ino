/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

#include "DSRtc.h"
#include "BTComms.h"
#include "Display.h"
#include "DFRobot_Heartrate.h"

DSRtc rtc;
#if defined(ARDUINO_SAM_DUE)
BTComms btComms(Serial1);       // setup the bluetooth with Serial port 1 for Arduino Due
#else
BTComms btComms(Serial);        // for other arduino's use the only serial port
#endif
Display display;
DFRobot_Heartrate heartSense(DIGITAL_MODE);
#define HEARTRATE_PIN   A0
#define BUFFER_SAMPLES   500
uint8_t bpmValue = 0;
static uint32_t deltaTime = 0;
uint16_t rateBuffer=0, idle=0 ,ThrownRate=0, SucessRate=0;          //Remove Thrown Rate once the system is working

uint16_t cumulativeRate=0;    
uint8_t bpm =0;

uint8_t StabiliseRate()
{
   
    if(millis()-deltaTime>20)
    {
        deltaTime=millis();
        heartSense.getValue(HEARTRATE_PIN);
        bpm =heartSense.getRate();
        if(rateBuffer<BUFFER_SAMPLES) 
        {
        
            if (bpm)
            {
                SucessRate++;
                //Serial.print(" Delta time: ");
                //Serial.print(deltaTime);
                Serial.print(" Number of Measurements: ");
                Serial.print(rateBuffer);
                Serial.print(" ,Number of Idles: ");
                Serial.print(idle);
                Serial.print(" ,Number of Thrown  Measurements: ");
                Serial.print(ThrownRate);
                Serial.print(" ,Number of Successful  Measurements: ");
                Serial.print(SucessRate);
                Serial.print(" ,Number of Cumulative  Measurements: ");
                Serial.print(cumulativeRate);
                Serial.print(" ,BPM: ");
                Serial.println(bpm);

                idle=0;
           
                cumulativeRate+=bpm;    
                       
            }
            else
            {
                //Serial.print(" Delta time: ");
                //Serial.print(deltaTime);
                Serial.print(" Number of Measurements: ");
                Serial.print(rateBuffer);
                Serial.print(" ,Number of Idles: ");
                Serial.print(idle);
                Serial.print(" ,Number of Thrown  Measurements: ");
                Serial.print(ThrownRate);
                Serial.print(" ,Number of Successful  Measurements: ");
                Serial.print(SucessRate);
                Serial.print(" ,Number of Cumulative  Measurements: ");
                Serial.print(cumulativeRate);
                Serial.print(" ,BPM: ");
                Serial.println(bpm);

                idle++;
                ThrownRate++;
            }             
            rateBuffer++; 
        } 
    }
    else
    {
        Serial.println("Waiting for delay");
    }
    
    if(rateBuffer==BUFFER_SAMPLES)
    {
        Serial.println("Buffer reset");
        rateBuffer=0;
        idle=0;
        ThrownRate=0;
        SucessRate=0;
        cumulativeRate=0;
    }

    if (idle<0.05*BUFFER_SAMPLES)
    {
        return cumulativeRate/SucessRate;
    }
    else
        return 0;           
}

void setup()
{
    Serial.begin(9600);               // begin serial for debug - this is used in other init functions to give current status
    //rtc.init();                         // init the rtc
    //btComms.init();                     // init the bt comms
   // display.init();                     // init the display - note this also runs multiple test functions
}

void loop()
{
    //DateTime now = rtc.get();
    //heartSense.getValue(HEARTRATE_PIN);
   // uint16_t val = analogRead(HEARTRATE_PIN);
    bpmValue = StabiliseRate();
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