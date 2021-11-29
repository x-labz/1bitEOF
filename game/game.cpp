
#include "game.h"
#include "./assets/cham_s.h"
#include "./assets/fly.h"
#include "game-types.h"
#include "button.h"
#include "Synth.h"

state_t state;

void initGame(void)
{
    state.fly = {130, 16, INIT, 0, 0, 1};
    state.cham = {IDLE, 0, 0, 1, 26, 16, 100, 0, 0};
}

void system_fly(state_t *state)
{
    fly_t *fly_p = &(state->fly);
    cham_t *cham_p = &(state->cham);

    if (fly_p->state == INIT)
    {
        fly_p->x = 130;
        fly_p->y = 5 + rand() % 25;
        fly_p->state = FLY;
        fly_p->dir = (rand() % 10) - 5;
        fly_p->frame = 0;
        fly_p->frame_dir = 1;

        cham_p->health -= 2;

        return;
    }
    if (fly_p->state == FLY)
    {
        fly_p->x--;
        fly_p->y += sgn(fly_p->dir);

        fly_p->dir += sgn(fly_p->dir) > 0 ? -1 : 1;

        if (fly_p->dir == 0)
        {
            fly_p->dir = (rand() % 10) - 5;
        }
        if (fly_p->y < 5)
        {
            fly_p->dir = 1;
        }
        if (fly_p->y > 28)
        {
            fly_p->dir = -1;
        }
        if (fly_p->x < 28)
        {
            fly_p->state = ESCAPED;
            return;
        }
    }
    if (fly_p->state == ESCAPED)
    {
        fly_p->x--;
        fly_p->y += sgn(fly_p->dir);
        if (fly_p->y > 32 || fly_p->y < 0 || fly_p->x < 0)
        {
            fly_p->state = INIT;
            return;
        }
    }
    if (fly_p->state == CATCHED)
    {
        fly_p->x = state->cham.offset_x + state->cham.tongue_x - 2;
    }
}

void systemAnimFly(state_t *state)
{
    fly_t *fly_p = &(state->fly);

    if (fly_p->state != CATCHED)
    {
        fly_p->frame += fly_p->frame_dir;
    }
    if (fly_p->frame == 1 || fly_p->frame == 0)
    {
        fly_p->frame_dir *= -1;
    }
}

void systemChamAction(state_t *state, uint8_t evt)
{
    uint8_t tounge_max = 60;
    cham_t *cham = &(state->cham);

    if (cham->state == DIED || cham->state == DYING)
        return;

    if (cham->state == IDLE && evt == CLK)
    {
        cham->state = OPENING;
        cham->health -= 10;
        setOSC(&osc1,1,3,0,0,1,46,127,160,39,1,59,0,0,13,1,0) ;
        return;
    }
    if (cham->state == OPENING)
    {
        if (cham->frame < 3)
        {
            cham->frame++;
        }
        else
        {
            cham->state = OPEN;
            return;
        }
    }

    if (cham->state == OPEN)
    {
        cham->tongue_x += TOUNGE_SPEED * cham->tounge_dir;
        if (cham->tongue_x > tounge_max)
        {
            cham->tounge_dir = -1;
        }
        if (cham->tongue_x <= 0)
        {
            cham->tounge_dir = 1;
            cham->tongue_x = 0;
            cham->state = CLOSING;
        }
        return;
    }

    if (cham->state == CLOSING)
    {
        if (state->fly.state == CATCHED)
        {
            state->fly.state = INIT;
            state->fly.x = 130;
        }
        if (cham->frame > 0)
        {
            cham->frame--;
        }
        else
        {
            cham->state = IDLE;
            return;
        }
    }
}

void systemDetectCatch(state_t *state)
{
    cham_t *cham_p = &(state->cham);
    fly_t *fly_p = &(state->fly);

    if (cham_p->state == OPEN && fly_p->state == FLY && (fly_p->y <= cham_p->offset_y) && (fly_p->y >= cham_p->offset_y - 3) && (cham_p->tongue_x + cham_p->offset_x) >= (fly_p->x - 1))
    {
        cham_p->tongue_x = fly_p->x - cham_p->offset_x  ;
        fly_p->state = CATCHED;
        cham_p->tounge_dir = -1;
        cham_p->fly_catched++;
        cham_p->health += 20;
    }
}

void systemCheckHealth(state_t *state)
{
    cham_t *cham_p = &(state->cham);

    if (cham_p->health > 100)
        cham_p->health = 100;

    if (cham_p->health < 1 && cham_p->tongue_x == 0 && cham_p->state == IDLE)
    {
        cham_p->health = 0;
        cham_p->state = DYING;
        cham_p->frame = 4;
    }

    if (cham_p->state == DYING)
    {
        cham_p->dying_state += 2;
        if (cham_p->dying_state >= sizeof(tounge_anim))
        {
            cham_p->state = DIED;
        }
    }
}

void restartGame(state_t *state, uint8_t evt)
{
    cham_t *cham_p = &(state->cham);
    if (cham_p->state == DIED && evt == CLK)
    {
        initGame();
    }
}

void renderSprite(int16_t x_pos, int16_t y_pos, const tImage *image, uint8_t w, uint8_t h, uint8_t offset, DisplayHal *display, bool fullScr)
{
    uint16_t o = offset * w * h;
    for (uint8_t y = 0; y != h; y++)
    {
        for (uint8_t x = 0; x != w; x++)
        {
            if (image->data[x + y * w + o])
            {
                display->drawPixel(x_pos + x, y_pos + y, SSD1306_WHITE, fullScr);
            }
        }
    }
}

void renderNum(uint8_t value, uint8_t x, uint8_t y, DisplayHal *display)
{
    char buffer[20];
    sprintf(buffer, "%d", value);
    for (uint8_t i = 0; i < 20 && buffer[i] != 0; i++)
    {
        uint8_t character = buffer[i];
        const tImage *img_p = Font_5x8.chars[character - 0x20].image;
        renderSprite(x + img_p->w * i, y, img_p, img_p->w, img_p->h, 0, display, false);
    }
}

void renderGame(state_t *state, DisplayHal *display)
{
    renderSprite(0, 0, &chamSprite, 32, 32, state->cham.frame, display, false);
    renderSprite(state->fly.x, state->fly.y, &flySprite, 6, 6, state->fly.frame, display, false);
    if (state->cham.tongue_x > 0)
    {
        for (uint8_t i = 0; i != state->cham.tongue_x; i++)
        {
            display->drawPixel(state->cham.offset_x + i, state->cham.offset_y, SSD1306_WHITE,false);
        }
    }

    cham_t *cham_p = &(state->cham);
    // draw died tounge
    if (cham_p->frame == 4)
    {
        for (uint8_t i = 0; i < cham_p->dying_state; i += 2)
        {
            display->drawPixel(cham_p->offset_x + tounge_anim[i], cham_p->offset_y + tounge_anim[i + 1], SSD1306_WHITE,false);
        }
    }
    // draw health meter
    const tImage *img_p = Font_5x8.chars[0x1b].image;
    renderSprite(70, 0, img_p, img_p->w, img_p->h, 0, display, false);
    if (cham_p->health > 0)
    {
        for (uint8_t i = 0; i != cham_p->health / 5; i++)
        {
            display->drawPixel(77 + i, 3, SSD1306_WHITE,false);
            display->drawPixel(77 + i, 4, SSD1306_WHITE,false);
        }
    }

    renderNum(cham_p->fly_catched, 100, 0, display);
}

void gameTick(DisplayHal *display, uint8_t event)
{

    display->clearDisplay();

    system_fly(&state);
    systemAnimFly(&state);
    systemChamAction(&state, event);
    systemDetectCatch(&state);
    systemCheckHealth(&state);
    restartGame(&state, event) ;
    renderGame(&state, display);

    display->display();
}