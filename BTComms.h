/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    BTComms.h
*/

#pragma once

#include "Arduino.h"

class BTComms
{
public:
    BTComms(HardwareSerial &BTSerialPort);

    void init();
    void status();
    void read();
    void write();

private:
    char hexToAscii(uint8_t d);
    uint8_t asciiToHex(char c);

private:
    HardwareSerial &m_BT;       // keep a reference to the hardware serial port we are using
};                              // this way we can refer to it without using confusing Serial'n' name