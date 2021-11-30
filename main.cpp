#include <Pokitto.h>
#include <LibLog>
#include "display_hal.h"
#include "button.h"
#include "./game/game.h"
#include "./assets/eof.h"
#include "font/Font.h"
#include "Synth.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
using PS = Pokitto::Sound ;

int main()
{
    PS::ampEnable(1);

    bool startup = true;

    PD::persistence = false;
    PD::invisiblecolor = 0;

    LOG("started \n");

    DisplayHal display;

    initGame();

    PC::begin();

    while (PC::isRunning())
    {
        if (!PC::update())
            continue;

        if (startup)
        {
            renderSprite(0, 0, &eofSprite, 220, 176, 0, &display, true);
            if (PC::buttons.bBtn()) startup = false ;
        }
        else
        {
            uint8_t evt = PC::buttons.aBtn() ? CLK : NONE;
            PD::color = 1;
            PD::drawRect((220 - 128) / 2 - 1, (176 - 32) / 2 - 1, 128 + 1, 32 + 1);
            gameTick(&display, evt);
        }
    }

    return 0;
}