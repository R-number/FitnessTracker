/*
Authour:        Team R-Number
Date Created:   25/02/2021
File name:      Lcd.cpp
*/

#include "Display.h"
#include "bitmaps.h"

// #define RUN_TEST 
// #define DEBUG_SERIAL        // note local only to this file

float p = 3.1415926;

Display::Display()
{
    /* empty constructor */
}

void Display::init() // the following code was taken from test.ino for the oled graphics library
{                    // run this init to test all the features
    m_oled.begin();
    Serial.println("init display");

    // You can optionally rotate the display by running the line below.
    // Note that a value of 0 means no rotation, 1 means 90 clockwise,
    // 2 means 180 degrees clockwise, and 3 means 270 degrees clockwise.
    //m_oled.setRotation(1);
    // NOTE: The test pattern at the start will NOT be rotated!  The code
    // for rendering the test pattern talks directly to the display and
    // ignores any rotation.

#ifdef RUN_TEST
    uint16_t time = millis();
    m_oled.fillRect(0, 0, 128, 128, BLACK);
    time = millis() - time;

    Serial.println(time, DEC);
    delay(500);

    testPattern();
    delay(500);

    m_oled.invert(true);
    delay(100);
    m_oled.invert(false);
    delay(100);

    m_oled.fillScreen(BLACK);
    testDrawText("Lorem ipsum dolor sit amet, consectetur adipiscing elit.\ 
    Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa,\ 
    fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque\ 
    ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa\
    eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", WHITE);
    delay(500);

    // m_oled print function!
    printTest();
    delay(500);

    //a single pixel
    m_oled.drawPixel(m_oled.width()/2, m_oled.height()/2, GREEN);
    delay(500);

    // line draw test
    testLines(YELLOW);
    delay(500);    

    // optimized lines
    testFastLines(RED, BLUE);
    delay(500);    


    testDrawRects(GREEN);
    delay(1000);

    testFillRects(YELLOW, MAGENTA);
    delay(1000);

    m_oled.fillScreen(BLACK);
    testFillCircles(10, BLUE);
    testDrawCircles(10, WHITE);
    delay(1000);

    testRoundRects();
    delay(500);

    testTriangles();
    delay(500);

    m_oled.fillScreen(BLACK);
    m_oled.drawBitmap(5, 5, heartLogo, 50, 50, BLACK, RED);
    delay(1000);
#endif

    m_oled.fillScreen(BLACK);
    Serial.println("done");
}

void Display::increment()
{
    static int oldMilis = 0, count = 0;

    m_oled.setCursor(1, 1);
    // if((millis() - oldMilis) > 2000)
    // {
    //     oldMilis = millis();
    //     count ++;
    //     m_oled.fillScreen(BLACK);
    // }
    m_oled.setTextColor(RED);
    m_oled.setTextSize(1);
    m_oled.print("Count: ");        // print every loop to catch spi on scope
    m_oled.print(count, DEC);

}

void Display::showTime(DateTime &t)
{
    static DateTime oldTime;    // note, may have to wait 1 min for initial update
    char buf[20] = {0};

    if(((t - oldTime).seconds() == 0) && (oldTime != t))
    {
        /* we get here if they are different */
        m_oled.setTextSize(3);

        m_oled.setCursor(2, 15);
        m_oled.setTextColor(BLACK);     // print the old string in the bg colour
        formatTime(buf, oldTime.hour(), oldTime.minute());
        m_oled.print(buf);              // this will erase the old time

        m_oled.setCursor(2, 15);        // reset the cursor
        m_oled.setTextColor(RED);
        formatTime(buf, t.hour(), t.minute());
        m_oled.print(buf);              // print the current buffer

    #ifdef DEBUG_SERIAL
        Serial.println(buf);
    #endif

        oldTime = t; 
    }
}

void Display::monitorTimeout(DateTime &t, uint16_t timeoutTime) // turns of the display after "timeoutTime" number of seconds
{                                                               // use to avoid screen burn in
    static DateTime oldTime = t;

    if((t-oldTime).totalseconds() > timeoutTime)
    {
        m_oled.enableDisplay(false);
    }
}

void Display::showSteps(uint16_t steps)
{
    static uint16_t oldSteps = 1;

    m_oled.drawBitmap(2, 90, shoeLogo, 24, 24, WHITE);

    if(oldSteps != steps)
    {
        m_oled.setTextSize(2);

        m_oled.setCursor(60, 95);
        m_oled.setTextColor(BLACK);
        m_oled.print(oldSteps, DEC);

        m_oled.setCursor(60, 95);
        m_oled.setTextColor(BLUE);
        m_oled.print(steps, DEC);

        oldSteps = steps;
    }
}

void Display::showHR(uint8_t bpm)
{
    static uint16_t oldBPM = 1;

    m_oled.drawBitmap(2, 50, heartLogo, 24, 24, RED);

    if(oldBPM != bpm)
    {
        m_oled.setTextSize(2);

        m_oled.setCursor(60, 55);
        m_oled.setTextColor(BLACK);
        m_oled.print(oldBPM, DEC);

        m_oled.setCursor(60, 55);
        m_oled.setTextColor(BLUE);
        m_oled.print(bpm, DEC);

        oldBPM = bpm;
    }
}

void Display::showGpsSignal(bool show)
{
    m_oled.setTextSize(1);
    m_oled.setCursor(110, 5);
    if(show)
    {
        m_oled.setTextColor(GREEN);
    }
    else
    {
        m_oled.setTextColor(RED);
    }
    m_oled.print("GPS");
}

void Display::showGpsData(float lat, float lng)
{
    static float oldLat, oldLong;

    m_oled.setTextSize(1);

    /* draw old stuff in black to erase */
    m_oled.setCursor(0, 40);
    m_oled.setTextColor(BLACK);
    m_oled.print("Lat: ");
    m_oled.println(oldLat);
    m_oled.print("Lng: ");
    m_oled.println(oldLong);    

    m_oled.setCursor(0, 40);
    m_oled.setTextColor(RED);
    m_oled.print("Lat: ");
    m_oled.println(lat);
    m_oled.print("Lng: ");
    m_oled.println(lng);

    oldLat = lat;
    oldLong = lng;
}

void Display::showGpsData(float lat, float lng, float alt)
{
    static float oldLat, oldLong, oldAlt;

    m_oled.setTextSize(1);

    /* draw old stuff in black to erase */
    m_oled.setCursor(0, 40);
    m_oled.setTextColor(BLACK);
    m_oled.print("Lat: ");
    m_oled.println(oldLat);
    m_oled.print("Lng: ");
    m_oled.println(oldLong);
    m_oled.print("Alt: ");
    m_oled.println(oldAlt);        

    m_oled.setCursor(0, 40);
    m_oled.setTextColor(RED);
    m_oled.print("Lat: ");
    m_oled.println(lat);
    m_oled.print("Lng: ");
    m_oled.println(lng);
    m_oled.print("Alt: ");
    m_oled.println(alt);

    oldLat = lat;
    oldLong = lng;
    oldAlt = alt;    
}

void Display::showGpsData(float lat, float lng, float alt, float dist)
{
    static float oldLat, oldLong, oldAlt, oldDist;

    m_oled.setTextSize(1);

    /* draw old stuff in black to erase */
    m_oled.setCursor(0, 40);
    m_oled.setTextColor(BLACK);
    m_oled.print("Lat: ");
    m_oled.println(oldLat);
    m_oled.print("Lng: ");
    m_oled.println(oldLong);
    m_oled.print("Alt: ");
    m_oled.println(oldAlt);
    m_oled.print("Dst: ");
    m_oled.println(oldDist);        

    m_oled.setCursor(0, 40);
    m_oled.setTextColor(RED);
    m_oled.print("Lat: ");
    m_oled.println(lat);
    m_oled.print("Lng: ");
    m_oled.println(lng);
    m_oled.print("Alt: ");
    m_oled.println(alt);
    m_oled.print("Dst: ");
    m_oled.println(dist); 

    oldLat = lat;
    oldLong = lng;
    oldAlt = alt;  
    oldDist = dist;   
}

void Display::testLines(uint16_t color)
{
    m_oled.fillScreen(BLACK);
    for (uint16_t x = 0; x < m_oled.width() - 1; x += 6)
    {
        m_oled.drawLine(0, 0, x, m_oled.height() - 1, color);
    }
    for (uint16_t y = 0; y < m_oled.height() - 1; y += 6)
    {
        m_oled.drawLine(0, 0, m_oled.width() - 1, y, color);
    }

    m_oled.fillScreen(BLACK);
    for (uint16_t x = 0; x < m_oled.width() - 1; x += 6)
    {
        m_oled.drawLine(m_oled.width() - 1, 0, x, m_oled.height() - 1, color);
    }
    for (uint16_t y = 0; y < m_oled.height() - 1; y += 6)
    {
        m_oled.drawLine(m_oled.width() - 1, 0, 0, y, color);
    }

    m_oled.fillScreen(BLACK);
    for (uint16_t x = 0; x < m_oled.width() - 1; x += 6)
    {
        m_oled.drawLine(0, m_oled.height() - 1, x, 0, color);
    }
    for (uint16_t y = 0; y < m_oled.height() - 1; y += 6)
    {
        m_oled.drawLine(0, m_oled.height() - 1, m_oled.width() - 1, y, color);
    }

    m_oled.fillScreen(BLACK);
    for (uint16_t x = 0; x < m_oled.width() - 1; x += 6)
    {
        m_oled.drawLine(m_oled.width() - 1, m_oled.height() - 1, x, 0, color);
    }
    for (uint16_t y = 0; y < m_oled.height() - 1; y += 6)
    {
        m_oled.drawLine(m_oled.width() - 1, m_oled.height() - 1, 0, y, color);
    }
}

void Display::testDrawText(char *text, uint16_t color)
{
    m_oled.setCursor(0, 0);
    m_oled.setTextColor(color);
    m_oled.print(text);
}

void Display::testFastLines(uint16_t color1, uint16_t color2)
{
    m_oled.fillScreen(BLACK);
    for (uint16_t y = 0; y < m_oled.height() - 1; y += 5)
    {
        m_oled.drawFastHLine(0, y, m_oled.width() - 1, color1);
    }
    for (uint16_t x = 0; x < m_oled.width() - 1; x += 5)
    {
        m_oled.drawFastVLine(x, 0, m_oled.height() - 1, color2);
    }
}

void Display::testDrawRects(uint16_t color)
{
    m_oled.fillScreen(BLACK);
    for (uint16_t x = 0; x < m_oled.height() - 1; x += 6)
    {
        m_oled.drawRect((m_oled.width() - 1) / 2 - x / 2, (m_oled.height() - 1) / 2 - x / 2, x, x, color);
    }
}

void Display::testFillRects(uint16_t color1, uint16_t color2)
{
    m_oled.fillScreen(BLACK);
    for (uint16_t x = m_oled.height() - 1; x > 6; x -= 6)
    {
        m_oled.fillRect((m_oled.width() - 1) / 2 - x / 2, (m_oled.height() - 1) / 2 - x / 2, x, x, color1);
        m_oled.drawRect((m_oled.width() - 1) / 2 - x / 2, (m_oled.height() - 1) / 2 - x / 2, x, x, color2);
    }
}

void Display::testFillCircles(uint8_t radius, uint16_t color)
{
    for (uint8_t x = radius; x < m_oled.width() - 1; x += radius * 2)
    {
        for (uint8_t y = radius; y < m_oled.height() - 1; y += radius * 2)
        {
            m_oled.fillCircle(x, y, radius, color);
        }
    }
}

void Display::testDrawCircles(uint8_t radius, uint16_t color)
{
    for (uint8_t x = 0; x < m_oled.width() - 1 + radius; x += radius * 2)
    {
        for (uint8_t y = 0; y < m_oled.height() - 1 + radius; y += radius * 2)
        {
            m_oled.drawCircle(x, y, radius, color);
        }
    }
}

void Display::testTriangles()
{
    m_oled.fillScreen(BLACK);
    int color = 0xF800;
    int t;
    int w = m_oled.width() / 2;
    int x = m_oled.height();
    int y = 0;
    int z = m_oled.width();
    for (t = 0; t <= 15; t += 1)
    {
        m_oled.drawTriangle(w, y, y, x, z, x, color);
        x -= 4;
        y += 4;
        z -= 4;
        color += 100;
    }
}

void Display::testRoundRects()
{
    m_oled.fillScreen(BLACK);
    int color = 100;

    int x = 0;
    int y = 0;
    int w = m_oled.width();
    int h = m_oled.height();
    for (int i = 0; i <= 24; i++)
    {
        m_oled.drawRoundRect(x, y, w, h, 5, color);
        x += 2;
        y += 3;
        w -= 4;
        h -= 6;
        color += 1100;
        Serial.println(i);
    }
}

void Display::printTest()
{
    m_oled.fillScreen(BLACK);
    m_oled.setCursor(0, 5);
    m_oled.setTextColor(RED);
    m_oled.setTextSize(1);
    m_oled.println("Hello World!");
    m_oled.setTextColor(YELLOW);
    m_oled.setTextSize(2);
    m_oled.println("Hello World!");
    m_oled.setTextColor(BLUE);
    m_oled.setTextSize(3);
    m_oled.print(1234.567);
    delay(1500);
    m_oled.setCursor(0, 5);
    m_oled.fillScreen(BLACK);
    m_oled.setTextColor(WHITE);
    m_oled.setTextSize(0);
    m_oled.println("Hello World!");
    m_oled.setTextSize(1);
    m_oled.setTextColor(GREEN);
    m_oled.print(p, 6);
    m_oled.println(" Want pi?");
    m_oled.println(" ");
    m_oled.print(8675309, HEX); // print 8,675,309 out in HEX!
    m_oled.println(" Print HEX!");
    m_oled.println(" ");
    m_oled.setTextColor(WHITE);
    m_oled.println("Sketch has been");
    m_oled.println("running for: ");
    m_oled.setTextColor(MAGENTA);
    m_oled.print(millis() / 1000);
    m_oled.setTextColor(WHITE);
    m_oled.print(" seconds.");
}

void Display::testPattern()
{
    static const uint16_t PROGMEM colors[] =
        {RED, YELLOW, GREEN, CYAN, BLUE, MAGENTA, BLACK, WHITE};

    for (uint8_t c = 0; c < 8; c++)
    {
        m_oled.fillRect(0, m_oled.height() * c / 8, m_oled.width(), m_oled.height() / 8,
                        pgm_read_word(&colors[c]));
    }
}

void Display::formatTime(char buf[], uint8_t h, uint8_t m)
{
    sprintf(buf, "%02u:%02u", h, m);
}

void Display::formatTime(char buf[], uint8_t h, uint8_t m, uint8_t s)
{
    sprintf(buf, "%02u:%02u:%02u", h, m, s);
}