#pragma once

#include <stdint.h>

typedef struct {
	const uint8_t *data;
    uint8_t w;
    uint8_t h;
    uint8_t block;
} tImage;


 typedef struct {
     long int code;
     const tImage *image;
     } tChar;

 typedef struct {
     int length;
     const tChar *chars;
     } tFont;


extern const tFont Font_5x8 ;
