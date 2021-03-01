/*
Authour:        Team R-Number
Date Created:   25/02/2021
File name:      Lcd.h
*/

#pragma once

/* 
NOTE: The following comment block described the
example sketch in which this test code was adapted from
*/

/*************************************************** 
 This is a example sketch demonstrating graphic drawing
capabilities of the SSD1351 library for the 1.5" 
and 1.27" 16-bit Color OLEDs with SSD1351 driver chip

Pick one up today in the adafruit shop!
------> http://www.adafruit.com/products/1431
------> http://www.adafruit.com/products/1673

If you're using a 1.27" OLED, change SCREEN_HEIGHT to 96 instead of 128.

These displays use SPI to communicate, 4 or 5 pins are required to  
interface
Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada for Adafruit Industries.  
BSD license, all text above must be included in any redistribution

The Adafruit GFX Graphics core library is also required
https://github.com/adafruit/Adafruit-GFX-Library
Be sure to install it!
****************************************************/

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <RTClib.h>      // for DateTime declration

// Screen dimensions
#define SCREEN_WIDTH  128
#define SCREEN_HEIGHT 128 // Change this to 96 for 1.27" OLED.

// You can use any (4 or) 5 pins 
// #define SCLK_PIN 2
// #define MOSI_PIN 3
#define DC_PIN   23
#define CS_PIN   22
#define RST_PIN  24

// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF

class Display
{
public:
    Display();
    void init();
    void increment();
    void showTime(DateTime &t);

private:

    /* --- test functions from test.ino --- */
    void testLines(uint16_t color);
    void testDrawText(char *text, uint16_t color);
    void testFastLines(uint16_t color1, uint16_t color2);
    void testDrawRects(uint16_t color);
    void testFillRects(uint16_t color1, uint16_t color2);
    void testFillCircles(uint8_t radius, uint16_t color);
    void testDrawCircles(uint8_t radius, uint16_t color);
    void testTriangles();
    void testRoundRects();
    void printTest();
    void testPattern();
    /* ----- */

    void formatTime(char buf[], uint8_t h, uint8_t m);
    void formatTime(char buf[], uint8_t h, uint8_t m, uint8_t s);

private:
    Adafruit_SSD1351 m_oled = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, 
                                                &SPI, CS_PIN, DC_PIN, RST_PIN);
};