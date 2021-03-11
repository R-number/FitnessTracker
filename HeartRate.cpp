/*
Authour:        Team R-Number
Date Created:   05/03/2021
File name:      HeartRate.cpp
*/

#include "HeartRate.h"

#define HEARTRATE_PIN   A0
#define BUFFER_SAMPLES   500
// #define DEBUG_SERIAL


HeartRate::HeartRate()
    : m_BPM(0), m_heartSense(DFRobot_Heartrate(DIGITAL_MODE))
{
    /* empty constructor */
}

void HeartRate::init()
{
    /* nothing to init... */
}

void HeartRate::loop()
{
    uint8_t tempBPM = stabilise();
    static uint32_t count = 0, deltaTime = 0;

    if(tempBPM)                 // if we get a non-zero bpm value
    {
        m_BPM = tempBPM;         // save it 
        count = 0;              // reset the count
    }
    else if(millis() - deltaTime > 1000)    // every second
    {
        deltaTime = millis();
        count++;                // increase the count if we get 0 bpm value
    }

    if(count >= 30)             // if still zero after 30 seconds
    {
        m_BPM = 0;              // reset the bpm
    }
}


uint8_t HeartRate::stabilise()
{
    static uint32_t deltaTime = 0;
    static uint16_t rateBuffer = 0, idle = 0 , ThrownRate = 0, SucessRate = 0;          //Remove Thrown Rate once the system is working
    static uint16_t cumulativeRate = 0;
    uint8_t bpm = 0;

    if(millis()-deltaTime>20)
    {
        deltaTime = millis();
        m_heartSense.getValue(HEARTRATE_PIN);
        bpm = m_heartSense.getRate();
        if(rateBuffer < BUFFER_SAMPLES) 
        {
            if (bpm)
            {
                SucessRate++;
                #ifdef DEBUG_SERIAL
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
                #endif

                idle=0;
                cumulativeRate += bpm;    
                       
            }
            else
            {
                #ifdef DEBUG_SERIAL
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
                #endif

                idle++;
                ThrownRate++;
            }             
            rateBuffer++; 
        } 
    }
    else
    {
        #ifdef DEBUG_SERIAL
        Serial.println("Waiting for delay");
        #endif
    }
    
    if(rateBuffer==BUFFER_SAMPLES)
    {
        #ifdef DEBUG_SERIAL
        Serial.println("Buffer reset");
        #endif
        rateBuffer=0;
        idle=0;
        ThrownRate=0;
        SucessRate=0;
        cumulativeRate=0;
    }

    if (idle < 0.05*BUFFER_SAMPLES && SucessRate >= 5)
    {
        return cumulativeRate/SucessRate;
    }
    else
    {
        return 0;
    }      
}