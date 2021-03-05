/*
Authour:        Team R-Number
Date Created:   05/03/2021
File name:      HeartRate.cpp
*/

#include "HeartRate.h"

#define HEARTRATE_PIN   A0
#define BUFFER_SAMPLES   200

HeartRate::HeartRate()
    : m_BPM(0)
{
    /* empty constructor */
}

void HeartRate::init()
{

}

void HeartRate::loop()
{

}

/* needs to be integrated */


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


    // if(millis() - deltaTime > 20)
    // {
    //     heartSense.getValue(HEARTRATE_PIN);
    //     uint8_t bpm = heartSense.getRate();

    //     if(bpm)
    //     {
    //         cumulativeRate += bpm;
    //         count++;
    //     }
    //     else
    //     {
    //         cumulativeRate = 0;
    //     }

    //     if(count >= 200)
    //     {
    //         cumulativeRate /= count;
    //     }
    // }

    // return cumulativeRate;
}