/*
Authour:        Team R-Number
Date Created:   05/03/2021
File name:      HeartRate.h
*/

#pragma once

#include <DFRobot_Heartrate.h>

class HeartRate
{
public:
    HeartRate();
    
    void init();
    void loop();

    void get() const { return m_BPM; }

private:
    uint8_t m_BPM;
    DFRobot_Heartrate m_heartSense(DIGITAL_MODE);
};