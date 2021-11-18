#pragma once 

#include <mbed.h>
#include "display_hal.h"

void initGame(void) ;
void gameTick(DisplayHal *display, uint8_t event) ;