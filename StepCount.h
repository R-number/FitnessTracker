/*
Authour:        Team R-Number
Date Created:   05/03/2021
File name:      StepCount.h
*/

#pragma once

#include <Wire.h>
#include <SparkFun_ADXL345.h>

class StepCount
{
public:
    StepCount();

    void init();
    void loop();

    uint16_t get() const { return m_steps; }

private:
    uint16_t m_steps;
    ADXL345 m_adxl = ADXL345();
};