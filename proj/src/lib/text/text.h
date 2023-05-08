#ifndef _PROJ_TEXT_H
#define _PROJ_TEXT_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    uint32_t width;
    uint32_t height;
} font_size_t;

typedef enum {
    small,
    medium,
    large,
} font_type_t;

uint8_t currentPixelSize;
font_size_t currentFontSize;
font_type_t currentFontType;

const bool alphabet[67][7][5];

int getCharIndex(const char c);
void setFontType(font_type_t fontType);

#endif // _PROJ_TEXT_H
