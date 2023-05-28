#ifndef _PROJ_GRAPHICS_H
#define _PROJ_GRAPHICS_H

#include <lcom/lcf.h>

vbe_mode_info_t modeInfo;
uint8_t* frameBuffer;
uint8_t* vMem;
size_t frameSize;
uint16_t bytesPP;

/**
 * @brief Sets the graphic mode with the specified submode.
 *
 * This function sets the graphic mode using the specified submode. 
 * @param submode The submode value to set for the graphic mode.
 * @return Non-zero if error occured. 
 */
int (set_graphic_mode)(uint16_t submode);
/**
 * @brief Sets the text mode.
 *
 * This function sets the minix mode to text mode. After setting the text mode, it clears the mode information structure.
 *
 * @return Non-zero if error occured. 
 */
int (set_text_mode)();

/**
 * @brief Sets up the frame buffer for the graphic mode.
 *
 * This function is called after the graphic mode has been set. It sets up the frame buffer by allocating physical and virtual memory.
 *
 * @return 0 if the frame buffer was set up successfully, otherwise 1 if an error occurred.
 */
int (set_frame_buffer)();

/**
 * @brief Exits the graphic mode and cleans up resources.

 * @return Non-zero if error occured. 
 */
int (exit_graphic_mode)();
/**
 * @brief Changes the current frame. 
 * 
 */
void (flip_frame)();

/**
 * @brief Draws a pixel of a specified color at the given coordinates.
 *
 * This function draws a pixel of the specified color at (x, y) coordinates on the screen. It first checks if the coordinates are within the screen resolution boundaries. It then calculates the index in the frame buffer based on the coordinates and the bytes per pixel and copies the color value into the frame buffer at the calculated index.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color value to be set for the pixel.
 * @return 0 if the pixel was drawn successfully, otherwise 1 if an error occurred.
 */
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
/**
 * @brief Draws a horizontal line of a specified color starting from the given coordinates.
 *
 * This function draws a horizontal line of the specified color starting from the given coordinates (x, y) with the specified length. It uses the vg_draw_pixel() function to draw each pixel along the line.
 *
 * @param x The starting x-coordinate of the line.
 * @param y The y-coordinate of the line.
 * @param len The length of the line in pixels.
 * @param color The color value to be set for each pixel in the line.
 * @return 0 if the line was drawn successfully, otherwise 1 if an error occurred.
 */
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
/**
 * @brief Draws a vertical line of a specified color starting from the given coordinates.
 *
 * This function draws a vertical line of the specified color starting from the given coordinates (x, y) with the specified length. It uses the vg_draw_pixel() function to draw each pixel along the line.
 *
 * @param x The starting x-coordinate of the line.
 * @param y The y-coordinate of the line.
 * @param len The length of the line in pixels.
 * @param color The color value to be set for each pixel in the line.
 * @return 0 if the line was drawn successfully, otherwise 1 if an error occurred.
 */
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);

/**
 * @brief Draws a filled rectangle of a specified color starting from the given coordinates.
 *
 * This function draws a filled rectangle of the specified color starting from the given coordinates (x, y) with the specified width and height. It uses the vg_draw_hline() function to draw each horizontal line of the rectangle by iterating over the y-axis.
 *
 * @param x The x-coordinate of the top-left corner of the rectangle.
 * @param y The y-coordinate of the top-left corner of the rectangle.
 * @param width The width of the rectangle in pixels.
 * @param height The height of the rectangle in pixels.
 * @param color The color value to be set for each pixel in the rectangle.
 * @return 0 if the rectangle was drawn successfully, otherwise 1 if an error occurred.
 */
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);

/**
 * @brief This function takes the individual red, green, and blue color components as input and combines them into a 32-bit color value according to the current mode information. 

 * @param red The red color component value (0-255).
 * @param green The green color component value (0-255).
 * @param blue The blue color component value (0-255).
 * @return The resulting 32-bit color value.
 */
uint32_t (rgbToColor)(uint8_t red, uint8_t green, uint8_t blue);

#endif // _PROJ_GRAPHICS_H
