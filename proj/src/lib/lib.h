#ifndef _PROJ_LIB_H
#define _PROJ_LIB_H

// LCOM Library
#include <lcom/lcf.h>

// Text Files
#include "text/text.h"

// Scancode Files
#include "scancode/scancode.h"

// Driver Files
#include <lcom/timer.h>
#include "drivers/timer/timer.h"
#include "drivers/graphics/graphics.h"
#include "drivers/KBC/keyboard.h"
#include "drivers/KBC/mouse.h"
#include "drivers/serialPort/serialPort.h"

uint16_t frameRate;
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

/**
 * @brief Initiates all device drivers.
 * 
 * @return Non-zero if error occured.
 */
int initAllDrivers();
/**
 * @brief Unsubscribes all devices' interrupts.
 * 
 * @return Non-zero if error occured.
 */
int exitAllDrivers();

/**
 * @brief Set the desired framerate.
 * 
 * @param fps 
 * @return Non-zero if error occured.
 */
int setFrameRate(uint16_t fps);
/**
 * @brief Set the desired minix mode.
 * 
 * @param mode 
 * @return Non-zero if error occured.
 */
int setMinixMode(minix_mode_t mode);
/**
 * @brief Exits graphics mode.
 * 
 * @param code 
 * @return Non-zero if error occured.
 */
int exitGraphMode(int code);
/**
 * @brief Sets application theme.
 * 
 * @param newTheme 
 * @return Non-zero if error occured.
 */
int setTheme(app_theme_t newTheme);

/**
 * @brief Gets x coordinate in pixels from a percentage.
 * 
 * @param px x percentage
 * @return x in pixels
 */
uint16_t getXFromPercent(float px);
/**
 * @brief Gets y coordinate in pixels from a percentage.
 * 
 * @param px x percentage
 * @return x in pixels
 */
uint16_t getYFromPercent(float py);
/**
 * @brief Clears the screen by clearing frame-buffer.
 * 
 */
void clearScreen  ();
/**
 * @brief Draws a colored pixel.
 * 
 * @param px 
 * @param py 
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.
 */
int drawPixelColor(float px, float py, uint32_t color);
/**
 * @brief Draws a colored pixel.
 * 
 * @param px 
 * @param py 
 * @param uint8_t red 
 * @param uint8_t green 
 * @param uint8_t blue 
 * @return Non-zero if error occured. 
 */
int drawPixelRGB  (float px, float py, uint8_t red, uint8_t green, uint8_t blue);
/**
 * @brief Draws a horizontal colored line.
 * 
 * @param px 
 * @param py 
 * @param len 
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.  
 */
int drawHLineColor(float px, float py, float len, uint32_t color);
/**
 * @brief Draws a horizontal colored line.
 * 
 * @param px 
 * @param py 
 * @param len 
 * @param uint8_t  red 
 * @param uint8_t  green 
 * @param uint8_t  blue 
 * @return Non-zero if error occured.   
 */
int drawHLineRGB  (float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue);
/**
 * @brief Draws a vertical colored line.
 * 
 * @param px 
 * @param py 
 * @param len 
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.   
 */
int drawVLineColor(float px, float py, float len, uint32_t color);
/**
 * @brief  Draws a vertical colored line.
 * 
 * @param px 
 * @param py 
 * @param len 
 * @param uint8_t red 
 * @param uint8_t green 
 * @param uint8_t blue 
 * @return  Non-zero if error occured.   
 */
int drawVLineRGB  (float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue);
/**
 * @brief Draws a rectangle receiving x and y in pixels.
 * 
 * @param x 
 * @param y 
 * @param width 
 * @param height 
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.  
 */
int _drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Draws a rectangle receiving x and y in percentage.
 * 
 * @param px 
 * @param py 
 * @param width 
 * @param height 
 * @param color  Format RGB (8 8 8)
 * @return Non-zero if error occured.  
 */
int drawRectColor (float px, float py, float width, float height, uint32_t color);
/**
 * @brief Draws a rect.
 * 
 * @param px 
 * @param py 
 * @param width 
 * @param height 
 * @param red 
 * @param green 
 * @param blue 
 * @return Non-zero if error occured.
 */
int drawRectRGB   (float px, float py, float width, float height, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Draws a 5 by 7 character.
 * 
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param c 
 * @param color 
 * @param size font size
 * @return Non-zero if error occured.
 */
int _draw5x7      (uint16_t x, uint16_t y, const bool c[7][5], uint32_t color, font_size_t size);
/**
 * @brief Draws a character. Only called when color is in the current mode format.
 * 
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param c 
 * @param color  mode formatted
 * @param size font size
 * @return Non-zero if error occured.
 */
int _drawChar     (uint16_t x, uint16_t y, const char c, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored character.
 * 
 * @param px percentage of x counting from top left corner.
 * @param py percentage of y counting from top left corner.
 * @param c 
 * @param color format RGB (8 8 8)
 * @param size  font size
 * @return Non-zero if error occured.
 */
int drawCharColor (float px, float py, const char c, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored character.
 * 
 * @param px percentage of x counting from top left corner.
 * @param py percentage of y counting from top left corner.
 * @param c 
 * @param uint8_t red 
 * @param uint8_t green 
 * @param uint8_t blue 
 * @param size font size
 * @return Non-zero if error occured.
 */
int drawCharRGB   (float px, float py, const char c, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
/**
 * @brief Draws a colored text of characters. Only called when color is in the current mode format. 
 * 
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param maxWidth
 * @param str 
 * @param color mode formatted
 * @param size font size
 * @return Non-zero if error occured. 
 */
int _drawText     (uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored text of characters.
 * 
 * @param cx x coordinate of the placed center
 * @param cy y coordinate of the placed center
 * @param maxWidth 
 * @param str 
 * @param color 
 * @param size 
 * @return Non-zero if error occured. 
 */
int drawTextColor (float cx, float cy, float maxWidth, const char* str, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored text of characters.
 * 
 * @param cx x coordinate of the placed center
 * @param cy y coordinate of the placed center
 * @param maxWidth 
 * @param str 
 * @param uint8_t red 
 * @param uint8_t green 
 * @param uint8_t blue 
 * @param size 
 * @return Non-zero if error occured.  
 */
int drawTextRGB   (float cx, float cy, float maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
/**
 * @brief  Draws a colored text of characters.
 * 
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param maxWidth 
 * @param str 
 * @param color format RGB (8 8 8)
 * @param size 
 * @return Non-zero if error occured. 
 */
int drawTextXYColor(uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size);
/**
 * @brief  Draws a colored text of characters.
 * 
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param maxWidth 
 * @param str 
 * @param uint8_t red 
 * @param uint8_t green 
 * @param uint8_t blue 
 * @param size 
 * @return Non-zero if error occured. 
 */
int drawTextXYRGB  (uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
/**
 * @brief Gets the text width
 * 
 * @param str 
 * @param size 
 * @return uint16_t width
 */
uint16_t getTextWidth(const char* str, font_size_t size);

#endif // _PROJ_LIB_H
