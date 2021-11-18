#include "display_hal.h"

using PD = Pokitto::Display;

void DisplayHal::clearDisplay(void){};
void DisplayHal::display(void){};
void DisplayHal::drawPixel(int16_t x, int16_t y, uint16_t color){
    uint16_t xOffset = (220 - 128) / 2 ;
    uint16_t yOffset = (176 - 32 ) / 2 ;
    PD::color = 1 ;
    PD::drawRect( xOffset -1, yOffset -1, 128 +1, 32 + 1 ) ;
    PD::drawPixel( xOffset +  x,  yOffset + y,color) ;
};
DisplayHal::DisplayHal(){} ;