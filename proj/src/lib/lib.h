#ifndef _PROJ_LIB_H
#define _PROJ_LIB_H

// LCOM Library
#include <lcom/lcf.h>

// Text Files
#include "text/text.h"

// Driver Files
#include <lcom/timer.h>
#include "drivers/timer/timer.h"
#include "drivers/graphics/graphics.h"
#include "drivers/KBC/keyboard.h"
#include "drivers/KBC/mouse.h"

// Macros
#include "drivers/graphics/VBE.h"
#include "drivers/timer/i8254.h"
#include "drivers/KBC/i8042.h"

message msg;
int ipcStatus;

typedef enum {
    textMode = 0,
    vbe768pInd = 0x105,
    vbe480pDc = 0x110,
    vbe600pDc = 0x115,
    vbe1024pDc = 0x11A,
    vbe864pDc = 0x14C
} minix_mode_t;

minix_mode_t currentMode;

int setFrameRate(uint16_t fps);
int setMinixMode(minix_mode_t mode);

void clearScreen();
int drawPixelColor(uint16_t x, uint16_t y, uint32_t color);
int drawPixelRGB(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue);
int drawHLineColor(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int drawHLineRGB(uint16_t x, uint16_t y, uint16_t len, uint8_t red, uint8_t green, uint8_t blue);
int drawVLineColor(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int drawVLineRGB(uint16_t x, uint16_t y, uint16_t len, uint8_t red, uint8_t green, uint8_t blue);
int drawRectColor(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int drawRectRGB(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue);
int _drawChar(uint16_t x, uint16_t y, const char c, uint32_t color);
int drawCharColor(uint16_t x, uint16_t y, const char c, uint32_t color);
int drawCharRGB(uint16_t x, uint16_t y, const char c, uint8_t red, uint8_t green, uint8_t blue);
int _drawText(uint16_t x, uint16_t y, const char* str, uint32_t color);
int drawTextColor(uint16_t x, uint16_t y, const char* str, uint32_t color);
int drawTextRGB(uint16_t x, uint16_t y, const char* str, uint8_t red, uint8_t green, uint8_t blue);

int initAll();
int exitAll();

#endif // _PROJ_LIB_H
