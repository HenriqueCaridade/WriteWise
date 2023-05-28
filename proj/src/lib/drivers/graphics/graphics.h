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
 * This function draws a pixel of the specified color at the given coordinates (x, y) on the screen. It first checks if the coordinates are within the screen resolution boundaries. If the coordinates are outside the boundaries, the function returns 1. Otherwise, it calculates the index in the frame buffer based on the coordinates and the bytes per pixel. It then copies the color value into the frame buffer at the calculated index.
 *
 * @param x The x-coordinate of the pixel.
 * @param y The y-coordinate of the pixel.
 * @param color The color value to be set for the pixel.
 * @return 0 if the pixel was drawn successfully, otherwise 1 if an error occurred.
 */
int (_vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, int retOutOfBounds);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
uint32_t (rgbToColor)(uint8_t red, uint8_t green, uint8_t blue);

#endif // _PROJ_GRAPHICS_H
