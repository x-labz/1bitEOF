#include <Pokitto.h>
#include <LibLog>
#include "display_hal.h"
#include "button.h"
#include "./game/game.h"

using PC = Pokitto::Core;
using PD = Pokitto::Display;
using PB = Pokitto::Buttons;
// using PS = Pokitto::Sound ;

int main()
{
    // PS::ampEnable(1);

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

        uint8_t evt = PC::buttons.aBtn() ? CLK : NONE;

        gameTick(&display, evt);
    }

    return 0;
}