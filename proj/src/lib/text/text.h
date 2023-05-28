#ifndef _PROJ_TEXT_H
#define _PROJ_TEXT_H

#include <stdint.h>
#include <stdbool.h>

/**
 * @struct font_size_t
 * @brief Struct that holds the font size information.
 * @var font_size_t::width 
 * Width of the character in this font size. (5 * pixel)
 * @var font_size_t::height
 * Height of the character in this font size. (7 * pixel)
 * @var font_size_t::pixel
 * Size of the pixel in this font
 */
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

/**
 * @brief Gets the index of the character in the alphabet array.
 * 
 * @param c character
 * @return index
 */
int getCharIndex(const char c);
/**
 * @brief Font-size getter.
 * 
 * @param fontType 
 * @return font_size_t 
 */
font_size_t getFontSize(font_type_t fontType);

#endif // _PROJ_TEXT_H
