#pragma once

#include <mbed.h>

#define TOUNGE_SPEED 5

enum flyStates_enum
{
    INIT,
    FLY,
    CATCHED,
    ESCAPED
};

typedef struct fly_t
{
    int16_t x;
    int16_t y;
    flyStates_enum state;
    int8_t dir;
    int8_t frame;
    int8_t frame_dir;
};

enum chamStates_emum
{
    IDLE,
    OPENING,
    OPEN,
    CLOSING,
    DYING,
    DIED
};

typedef struct cham_t
{
    chamStates_emum state;
    int8_t frame;
    int8_t tongue_x;
    int8_t tounge_dir;
    int8_t offset_x;
    int8_t offset_y;
    int8_t health;
    uint16_t fly_catched;
    uint8_t dying_state;
};

typedef struct state_t
{
    fly_t fly;
    cham_t cham;
};

template <typename T>
int sgn(T val)
{
    return (T(0) < val) - (val < T(0));
}

static const uint8_t tounge_anim[] = { 0, 0, 1, 0, 2, 0, 3, 1, 4, 2, 4, 3, 4, 4, 4, 5, 4, 6, 4, 7, 4, 8, 4, 9, 4, 10, 4, 11, 4, 12, 4, 13, 5, 14, 6, 15, 7, 15, 8, 15, 9, 15, 10, 15 };
