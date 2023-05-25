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

typedef enum {
    darkTheme,
    lightTheme
} app_theme_t;
app_theme_t currentTheme;

int initAllDrivers();
int exitAllDrivers();

int setFrameRate(uint16_t fps);
int setMinixMode(minix_mode_t mode);
int exitGraphMode(int code);
int setTheme(app_theme_t newTheme);

uint16_t getXFromPercent(float px);
uint16_t getYFromPercent(float py);

void clearScreen  ();
int drawPixelColor(float px, float py, uint32_t color);
int drawPixelRGB  (float px, float py, uint8_t red, uint8_t green, uint8_t blue);
int drawHLineColor(float px, float py, float len, uint32_t color);
int drawHLineRGB  (float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue);
int drawVLineColor(float px, float py, float len, uint32_t color);
int drawVLineRGB  (float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue);
int _drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
int drawRectColor (float px, float py, float width, float height, uint32_t color);
int drawRectRGB   (float px, float py, float width, float height, uint8_t red, uint8_t green, uint8_t blue);
int _draw5x7      (uint16_t x, uint16_t y, const bool c[7][5], uint32_t color, font_size_t size);
int _drawChar     (uint16_t x, uint16_t y, const char c, uint32_t color, font_size_t size);
int drawCharColor (float px, float py, const char c, uint32_t color, font_size_t size);
int drawCharRGB   (float px, float py, const char c, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
int _drawText     (uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size);
int drawTextColor (float cx, float cy, float maxWidth, const char* str, uint32_t color, font_size_t size);
int drawTextRGB   (float cx, float cy, float maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
int drawTextXYColor(uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size);
int drawTextXYRGB  (uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
uint16_t getTextWidth(const char* str, font_size_t size);

#endif // _PROJ_LIB_H
