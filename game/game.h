#pragma once 

#include <mbed.h>
#include "display_hal.h"
#include "font/Font.h"

void initGame(void) ;
void gameTick(DisplayHal *display, uint8_t event) ;
void renderSprite(int16_t x_pos, int16_t y_pos, const tImage *image, uint8_t w, uint8_t h, uint8_t offset, DisplayHal *display, bool fullScr);