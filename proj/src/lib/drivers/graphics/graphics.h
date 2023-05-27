#ifndef _PROJ_GRAPHICS_H
#define _PROJ_GRAPHICS_H

#include <lcom/lcf.h>

vbe_mode_info_t modeInfo;
uint8_t* frameBuffer;
uint8_t* vMem;
size_t frameSize;
uint16_t bytesPP;

int (set_graphic_mode)(uint16_t submode);
int (set_text_mode)();
int (set_frame_buffer)();
int (exit_graphic_mode)();
void (flip_frame)();

int (_vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color, int retOutOfBounds);
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
uint32_t (rgbToColor)(uint8_t red, uint8_t green, uint8_t blue);

#endif // _PROJ_GRAPHICS_H
