/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    BTComms.cpp
*/

#include "BTComms.h"

#define STATUS_PIN  27
#define EN_PIN      28

BTComms::BTComms(HardwareSerial &BTSerialPort)
    :m_BT(BTSerialPort)

{
    /* empty constructor */
}

void BTComms::init()
{
    m_BT.begin(9600);            // begin comms with HC-05 module, default baud rate is 9600
    Serial.println("init BT");
    pinMode(STATUS_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);
    pinMode(EN_PIN, OUTPUT);
}

void BTComms::status()
{
    digitalWrite(LED_BUILTIN, digitalRead(STATUS_PIN));  // set the led based on the HC-05 current status
}

void BTComms::read()
{
    if(m_BT.available())
    {
        Serial.write(m_BT.read());  // read whatever is incoming from BT and write to debug serial port
    }
}

void BTComms::write()
{
        if(Serial.available())
    {
        m_BT.write(Serial.read());  // read whatever is coming from Serial and write to BT
    }
}
