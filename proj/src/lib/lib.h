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
#include "drivers/rtc/rtc.h"

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
 * This function initializes the drivers for all devices used in the application.
 * @return Non-zero if error occured.
 */
int initAllDrivers();
/**
 * @brief Unsubscribes all devices' interrupts.
 * 
 * This function unsubscribes the interrupts of all devices used in the application, including the serial port, mouse, keyboard, and timer.
 * @return Non-zero if error occured.
 */
int exitAllDrivers();

/**
 * @brief Set the desired framerate.
 * 
 * This function sets the desired frame rate based on the provided `fps` parameter.
 * @param fps desired frame rate to set.
 * @return Non-zero if error occured.
 */
int setFrameRate(uint16_t fps);
/**
 * @brief Set the desired minix mode.
 * 
 * This function sets the desired Minix mode based on the provided `mode` parameter.
 * @param mode desired Minix mode to set.
 * @return Non-zero if error occured.
 */
int setMinixMode(minix_mode_t mode);
/**
 * @brief Exits graphics mode.
 * 
 * This function attempts to exit the graphics mode by calling the `exit_graphic_mode` function multiple times with a delay between each attempt.
 * @param code exit code to return.
 * @return Non-zero if error occured.
 */
int exitGraphMode(int code);
/**
 * @brief Sets application theme.
 * 
 * This function sets the theme of the application to the specified newTheme value.
 * @param newTheme new theme value to set.
 * @return Non-zero if error occured.
 */
int setTheme(app_theme_t newTheme);

/**
 * @brief Gets x coordinate in pixels from a percentage.
 * 
 * This function calculates the x coordinate in pixels based on the given percentage value (px) and the current display mode.
 * @param px x coordinate in percentage.
 * @return x in pixels
 */
uint16_t getXFromPercent(float px);
/**
 * @brief Gets y coordinate in pixels from a percentage.
 * 
 * This function calculates the y coordinate in pixels based on the given percentage value (py) and the current display mode.
 * @param py y coordinate in percentage.
 * @return y in pixels
 */
uint16_t getYFromPercent(float py);
/**
 * @brief Clears the screen by clearing frame-buffer.
 * 
 * This function clears the screen by setting all the pixels in the frame buffer to either 0x00 (dark theme) or 0xFF (light theme), depending on the current theme.
 */
void clearScreen  ();
/**
 * @brief Draws a colored pixel.
 * 
 * This function draws a pixel at the specified coordinates `px` and `py` in percentage values.
 * @param px x coordinate of the pixel, in percentage.
 * @param py y coordinate of the pixel, in percentage.
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.
 */
int drawPixelColor(float px, float py, uint32_t color);
/**
 * @brief Draws a colored pixel.
 * 
 * This function draws a pixel at the specified coordinates `px` and `py` in percentage values.
 * @param px x coordinate of the pixel, in percentage.
 * @param py y coordinate of the pixel, in percentage.
 * @param red red component of the pixel color.
 * @param green green component of the pixel color.
 * @param blue blue component of the pixel color.
 * @return Non-zero if error occured. 
 */
int drawPixelRGB  (float px, float py, uint8_t red, uint8_t green, uint8_t blue);
/**
 * @brief Draws a horizontal colored line.
 * 
 * This function draws a horizontal line starting from the position specified by `px` and `py` in percentage values.
 * @param px x coordinate of the starting point of the line, in percentage.
 * @param py y coordinate of the starting point of the line, in percentage.
 * @param len The length of the line.
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.  
 */
int drawHLineColor(float px, float py, float len, uint32_t color);
/**
 * @brief Draws a horizontal colored line.
 * 
 * This function draws a horizontal line starting from the position specified by `px` and `py` in percentage values.
 * @param px x coordinate of the starting point of the line, in percentage.
 * @param py y coordinate of the starting point of the line, in percentage.
 * @param len The length of the line.
 * @param red red component of the line color.
 * @param green green component of the line color.
 * @param blue blue component of the line color.
 * @return Non-zero if error occured.   
 */
int drawHLineRGB  (float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue);
/**
 * @brief Draws a vertical colored line.
 * 
 * This function draws a vertical line starting from the position specified by `px` and `py` in percentage values.
 * @param px x coordinate of the starting point of the line, in percentage.
 * @param py y coordinate of the starting point of the line, in percentage.
 * @param len The length of the line.
 * @param color Format RGB (8 8 8).
 * @return Non-zero if error occured.   
 */
int drawVLineColor(float px, float py, float len, uint32_t color);
/**
 * @brief  Draws a vertical colored line.
 * 
 * This function draws a vertical line starting from the position specified by `px` and `py` in percentage values.
 * @param px x coordinate of the starting point of the line, in percentage.
 * @param py y coordinate of the starting point of the line, in percentage.
 * @param len The length of the line.
 * @param red red component of the line color.
 * @param green green component of the line color.
 * @param blue blue component of the line color.
 * @return  Non-zero if error occured.   
 */
int drawVLineRGB  (float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue);
/**
 * @brief Draws a rectangle receiving x and y in pixels.
 * 
 * This function draws a rectangle at the given `x` and `y` coordinates in pixels.
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param width width of the rectangle, in pixels.
 * @param height height of the rectangle, in pixels.
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.  
 */
int _drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
/**
 * @brief Draws a rectangle receiving x and y in percentage.
 * 
 * This function draws a rectangle at the specified position and dimensions.
 * @param px x coordinate of the top-left corner of the rectangle, in pixels counting from the top left corner.
 * @param py y coordinate of the top-left corner of the rectangle, in pixels counting from the top left corner.
 * @param width width of the rectangle, in pixels.
 * @param height height of the rectangle, in pixels.
 * @param color Format RGB (8 8 8)
 * @return Non-zero if error occured.  
 */
int drawRectColor (float px, float py, float width, float height, uint32_t color);
/**
 * @brief Draws a rect.
 * 
 * This function draws a rectangle at the specified position and dimensions.
 * @param px x coordinate of the top-left corner of the rectangle, in pixels counting from the top left corner.
 * @param py y coordinate of the top-left corner of the rectangle, in pixels counting from the top left corner.
 * @param width width of the rectangle, in pixels.
 * @param height height of the rectangle, in pixels.
 * @param red red component of the rectangle's color.
 * @param green green component of the rectangle's color.
 * @param blue blue component of the rectangle's color.
 * @return Non-zero if error occured.
 */
int drawRectRGB   (float px, float py, float width, float height, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Draws a 5 by 7 character.
 * 
 * This function draws a 5 by 7 character at the specified coordinates.
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param c boolean array representing the 5 by 7 character.
 * @param color the color of the character.
 * @param size font size
 * @return Non-zero if error occured.
 */
int _draw5x7      (uint16_t x, uint16_t y, const bool c[7][5], uint32_t color, font_size_t size);
/**
 * @brief Draws a character. Only called when color is in the current mode format.
 * 
 * This function draws a single character at the specified coordinates using the current color format. 
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param c the character to be drawn.
 * @param color  mode formatted
 * @param size font size
 * @return Non-zero if error occured.
 */
int _drawChar     (uint16_t x, uint16_t y, const char c, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored character at the specified coordinates.
 * 
 * This function draws a single character with the specified color and font size at the given coordinates.
 * @param px percentage of x counting from top left corner.
 * @param py percentage of y counting from top left corner.
 * @param c represents the character to be drawn.
 * @param color format RGB (8 8 8).
 * @param size  font size.
 * This function checks the current display mode and uses the appropriate color format for drawing the character. If the current mode is in Indexed Mode, it directly uses the color parameter. Otherwise, it converts the RGB color components extracted from the color parameter using bitwise operations and passes it to the underlying drawing function
 * @return Non-zero if error occured.
 */
int drawCharColor (float px, float py, const char c, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored character.
 * 
 * This function draws a single character with the specified color and font size at the given coordinates.
 * @param px percentage of x counting from top left corner.
 * @param py percentage of y counting from top left corner.
 * @param c character to be drawn.
 * @param red red component of the color.
 * @param green green component of the color.
 * @param blue blue component of the color.
 * @param size font size.
 * @return Non-zero if error occured.
 */
int drawCharRGB   (float px, float py, const char c, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
/**
 * @brief Draws a colored text of characters. Only called when color is in the current mode format. 
 * 
 * This function draws a text string with the specified color and font size at the given coordinates.
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param maxWidth maximum width of the text.
 * @param str text string to be drawn.
 * @param color mode formatted.
 * @param size font size.
 * @return Non-zero if error occured. 
 */
int _drawText     (uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored text of characters.
 * 
 * This function draws a text string with the specified color and font size at the given coordinates.
 * @param cx x coordinate of the placed center.
 * @param cy y coordinate of the placed center.
 * @param maxWidth maximum width of the text.
 * @param str text string to be drawn.
 * @param color mode formatted.
 * @param size font size.
 * @return Non-zero if error occured. 
 */
int drawTextColor (float cx, float cy, float maxWidth, const char* str, uint32_t color, font_size_t size);
/**
 * @brief Draws a colored text of characters.
 * 
 * This function draws a text string with the specified color and font size at the given coordinates.
 * @param cx x coordinate of the placed center
 * @param cy y coordinate of the placed center
 * @param maxWidth maximum width of the text.
 * @param str text string to be drawn.
 * @param red red component of the color.
 * @param green green component of the color.
 * @param blue blue component of the color.
 * @param size font size.
 * @return Non-zero if error occured.  
 */
int drawTextRGB   (float cx, float cy, float maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
/**
 * @brief  Draws a colored text of characters.
 * 
 * This function draws a text string with the specified color and font size at the given coordinates.
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param maxWidth maximum width of the text.
 * @param str text string to be drawn.
 * @param color format RGB (8 8 8)
 * @param size font size.
 * @return Non-zero if error occured. 
 */
int drawTextXYColor(uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size);
/**
 * @brief  Draws a colored text of characters.
 * 
 * This function draws a text string with the specified color and font size at the given coordinates.
 * @param x in pixels counting from top left corner.
 * @param y in pixels counting from top left corner.
 * @param maxWidth maximum width of the text.
 * @param str text string to be drawn.
 * @param red red component of the color.
 * @param green green component of the color.
 * @param blue blue component of the color.
 * @param size font size.
 * @return Non-zero if error occured. 
 */
int drawTextXYRGB  (uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size);
/**
 * @brief Gets the text width
 * 
 * This function calculates the width of the text string by multiplying the length of the string with the total width of each character, including the pixel spacing.
 * @param str text string to be drawn.
 * @param size font size.
 * @return uint16_t width
 */
uint16_t getTextWidth(const char* str, font_size_t size);

char *_getRealTime();
int drawRealTime(uint32_t color, font_size_t size);


#endif // _PROJ_LIB_H
