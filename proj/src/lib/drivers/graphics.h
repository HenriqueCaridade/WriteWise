#ifndef _PROJ_GRAPHICS_H
#define _PROJ_GRAPHICS_H

#include <lcom/lcf.h>
#include "VBE.h"

vbe_mode_info_t modeInfo;
uint8_t* frameBuffer;
uint16_t bytes_pp;

int (set_graphic_mode)(uint16_t submode);
int (set_text_mode)();
int (set_frame_buffer)();
int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color);
int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color);
int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color);
uint32_t (normalize_color)(uint32_t color);
uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B);
uint32_t (R)(uint32_t pixel);
uint32_t (G)(uint32_t pixel);
uint32_t (B)(uint32_t pixel);

#endif // _PROJ_GRAPHICS_H
