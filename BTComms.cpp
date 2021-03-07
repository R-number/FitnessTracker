/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    BTComms.cpp
*/

#include "BTComms.h"

#define STATUS_PIN  27
#define EN_PIN      28

#define DEBUG_SERIAL        // comment out to stop debug serial prints

BTComms::BTComms(HardwareSerial &BTSerialPort, DSRtc &rtcRef, 
                GPS &gpsRef, StepCount &scRef, HeartRate &hrRef)
    :m_BT(BTSerialPort), m_rxLen(0), m_rtcRef(rtcRef), m_gpsRef(gpsRef),
    m_scRef(scRef), m_hrRef(hrRef)
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

void BTComms::loop()
{
    bool newData = false;

    while(m_BT.available() > 0)
    {
        m_rxLen = m_BT.readBytesUntil('Z', m_rxBuf, 1024);
        m_BT.read();    // read until the terminator then clear the buffer (we ignore anything else thats in there)
        newData = true;
    }

    if(newData)
    {
        Serial.print("Rxlen: ");
        Serial.println(m_rxLen);

        #ifdef DEBUG_SERIAL
        for(uint16_t i=0; i<m_rxLen; i++)
        {
            Serial.println(m_rxBuf[i]);
        }
        #endif

        parse();
    }
}

void BTComms::read()
{
    if(m_BT.available() > 0)
    {
        Serial.write(Serial.read());
    }
}

void BTComms::write()
{
    if(Serial.available() > 0)
    {
        m_BT.write(Serial.read());  // read whatever is coming from Serial and write to BT
    }
}

void BTComms::parse()
{
    nibbles32_t tempTime = {0};
    nibbles16_t tempDist = {0}, tempHR = {0}, tempSteps = {0};

    switch(m_rxBuf[0])
    {
        case 'T':
            tempTime.NIBBLE7 = asciiToHex(m_rxBuf[1]);
            tempTime.NIBBLE6 = asciiToHex(m_rxBuf[2]);
            tempTime.NIBBLE5 = asciiToHex(m_rxBuf[3]);
            tempTime.NIBBLE4 = asciiToHex(m_rxBuf[4]);
            tempTime.NIBBLE3 = asciiToHex(m_rxBuf[5]);
            tempTime.NIBBLE2 = asciiToHex(m_rxBuf[6]);
            tempTime.NIBBLE1 = asciiToHex(m_rxBuf[7]);
            tempTime.NIBBLE0 = asciiToHex(m_rxBuf[8]);

            m_rtcRef.set(tempTime.VAL);                 // set the time
            m_BT.write("TZ\n");                         // reply to verify we recieved the time
            break;
        
        case 'D':
            tempDist = {.VAL = m_gpsRef.getDistance()*1000};
            m_BT.write("D");
            m_BT.write(hexToAscii(tempDist.NIBBLE3));
            m_BT.write(hexToAscii(tempDist.NIBBLE2));
            m_BT.write(hexToAscii(tempDist.NIBBLE1));
            m_BT.write(hexToAscii(tempDist.NIBBLE0));
            m_BT.write("Z\n");
            break;

        case 'H':
            tempHR = {.VAL = m_hrRef.get()};
            m_BT.write("H");
            m_BT.write(hexToAscii(tempHR.NIBBLE3));
            m_BT.write(hexToAscii(tempHR.NIBBLE2));
            m_BT.write(hexToAscii(tempHR.NIBBLE1));
            m_BT.write(hexToAscii(tempHR.NIBBLE0));
            m_BT.write("Z\n");
            break;
        
        case 'S':
            tempSteps = {.VAL = m_scRef.get()};
            m_BT.write("S");
            m_BT.write(hexToAscii(tempSteps.NIBBLE3));
            m_BT.write(hexToAscii(tempSteps.NIBBLE2));
            m_BT.write(hexToAscii(tempSteps.NIBBLE1));
            m_BT.write(hexToAscii(tempSteps.NIBBLE0));
            m_BT.write("Z\n");
            break;
        
        default:
            Serial.println("Error, unidenfitied message received!");
            break;
    }
}

char BTComms::hexToAscii(uint8_t d)
{
	if(d < 10)
		d += '0';
	else
		d += ('A' - 10);
	return d;
}

uint8_t BTComms::asciiToHex(char c)
{
	if(c > '9')
		c -= ('A' - 10);
	else
		c -= '0';
	return c;
}