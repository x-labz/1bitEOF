#include "display_hal.h"

using PD = Pokitto::Display;

void DisplayHal::clearDisplay(void){};
void DisplayHal::display(void){};
void DisplayHal::drawPixel(int16_t x, int16_t y, uint16_t color, bool fullScr){
    uint16_t xOffset = !fullScr ? (220 - 128) / 2 : 0 ;
    uint16_t yOffset = !fullScr ? (176 - 32 ) / 2 : 0 ;
    PD::color = 1 ;
    PD::drawPixel( xOffset +  x,  yOffset + y,color) ;
};
DisplayHal::DisplayHal(){} ;