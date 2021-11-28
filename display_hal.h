#pragma once

#include <mbed.h>
#include <Pokitto.h>

#define SSD1306_WHITE 1

class DisplayHal
{

public:
    DisplayHal();
    void clearDisplay(void);
    void display(void);
    void drawPixel(int16_t x, int16_t y, uint16_t color, bool fullScr);
};