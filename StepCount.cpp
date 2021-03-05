/*
Authour:        Team R-Number
Date Created:   05/03/2021
File name:      StepCount.cpp
*/

#include "StepCount.h"

#define DEBUG_SERIAL
// #define BEBUG_LED

StepCount::StepCount()
    : m_steps(0)
{
    /* empty constructor */
}

void StepCount::init()
{
    const int range_setting = 8;       // Sensitivity of the m_adxl345 (2, 4, 8 or 16G)
    const int tap_threshold = 60;      // Force required to count as a step (in mG through trial and error)
    const int tap_duration = 150;      // 625 μs per increment (duration of tap)

    m_adxl.powerOn();                       // Start the accelerometer
    m_adxl.setRangeSetting(range_setting);
    m_adxl.setSpiBit(0);
    m_adxl.setActivityXYZ(0, 0, 0);         // set x, y, z parameters to relative 0
    m_adxl.setInactivityXYZ(0, 0, 0);
    m_adxl.setTapDetectionOnXYZ(0, 0, 1);   // detect if the device if tapped while on a flat surface
    m_adxl.setTapThreshold(tap_threshold);
    m_adxl.setTapDuration(tap_duration);
    m_adxl.InactivityINT(0);                // detect if no "taps" are detected for a period of time, 
    m_adxl.ActivityINT(0);
    m_adxl.FreeFallINT(0);                  // if it is being dropped, ignore it as a step
    m_adxl.doubleTapINT(0);                 // do nothing if "double-tapped"
    m_adxl.singleTapINT(1);                 // count as a step if "tapped" once
}

void StepCount::loop()
{
    byte interrupts = m_adxl.getInterruptSource();
    if(m_adxl.triggered(interrupts, ADXL345_SINGLE_TAP))
    {  
        #ifdef DEBUG_LED
        digitalWrite(LED_BUILTIN, HIGH);        // Blinks the LED (in ms)
        delay(250);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
        #endif

        m_steps++;

        #ifdef DEBUG_SERIAL
        Serial.print("Steps: ");
        Serial.println(m_steps);
        #endif
    }
}