/**
* Authour:      Team R-Number
* Date Created: 24/02/21
* File name:    FitnessTracker.ino
*/

//#include "DSRtc.h"
//#include "BTComms.h"
#include <Wire.h>
#include "Display.h"
#include <SPI.h>
#include <EEPROM.h>
#include <SparkFun_ADXL345.h>

//DSRtc rtc;
//BTComms btComms(Serial1);             //setup the bluetooth with Serial port 1
Display display;

const int chip_select_pin = 10;    // CS pin for SPI
const int led_pin = 2;             // The LED blinks when a step is detected
const int jumper_pin = 3;          // Put this pin low to read the step count
const int eeprom_address = 0x01;   // Address where to store the step count
const int range_setting = 8;       // Sensitivity of the ADXL345 (2, 4, 8 or 16G)
const int tap_threshold = 60;      // Force required to count as a step (in mG through trial and error)
const int tap_duration = 150;      // 625 μs per increment (duration of tap)

unsigned int step_number = 0;      // Total step count initialised at 0

ADXL345 adxl = ADXL345(chip_select_pin);
//int ADXL345 = 0x53;

void setup()
{
  Serial.begin(9600);       //[115200]begin serial for debug - this is used in other init functions to give current status
  //rtc.init();               //initialise the rtc
  //btComms.init();         //init the bt comms
  //display.init();           //init the display - note this also runs multiple test functions

  EEPROM.put(eeprom_address, 0);        // Reset the step count in eeprom

  adxl.powerOn();                       // Start the accelerometer
  adxl.setRangeSetting(range_setting);
  adxl.setSpiBit(0);
  adxl.setActivityXYZ(0, 0, 0);         // set x, y, z parameters to relative 0
  adxl.setInactivityXYZ(0, 0, 0);
  adxl.setTapDetectionOnXYZ(0, 0, 1);   // detect if the device if tapped while on a flat surface
  adxl.setTapThreshold(tap_threshold);
  adxl.setTapDuration(tap_duration);
  adxl.InactivityINT(0);                // detect if no "taps" are detected for a period of time, 
  adxl.ActivityINT(0);
  adxl.FreeFallINT(0);                  // if it is being dropped, ignore it as a step
  adxl.doubleTapINT(0);                 // do nothing if "double-tapped"
  adxl.singleTapINT(1);                 // count as a step if "tapped" once

  pinMode(led_pin, OUTPUT);
  digitalWrite(led_pin, LOW);
}

void loop()
{
  byte interrupts = adxl.getInterruptSource();
  if(adxl.triggered(interrupts, ADXL345_SINGLE_TAP))
  {
    //DateTime now = rtc.get();
    //display.showTime(now);

    //delay(500);
    //btComms.read();
    //btComms.write();
    //display.increment();

    // Serial.print("Heartrate val: ");    //read heartrate and print to console
    // Serial.println(analogRead(HEARTRATE_PIN));
    
    //Serial.println("STEP");             // test to see if steps are being registered
    
    digitalWrite(led_pin, HIGH);        // Blinks the LED (in ms)
    delay(250);
    digitalWrite(led_pin, LOW);
    delay(100);

    EEPROM.put(eeprom_address, step_number++);
    Serial.print("Steps: ");
    Serial.println(step_number);
  }
}
