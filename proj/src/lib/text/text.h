#ifndef _PROJ_TEXT_H
#define _PROJ_TEXT_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t pixel;
} font_size_t;

typedef enum {
    small,
    medium,
    large,
    xlarge,
    xxlarge,
    xxxlarge,
} font_type_t;

#define ALPHABET_SIZE 96
const bool alphabet[ALPHABET_SIZE][7][5];

int getCharIndex(const char c);
font_size_t getFontSize(font_type_t fontType);

#endif // _PROJ_TEXT_H
