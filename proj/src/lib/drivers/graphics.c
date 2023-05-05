#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>

int (set_graphic_mode)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = submode | BIT(14);
    if (sys_int86(&reg86) != 0) {
        printf("Set graphic mode failed.\n");
        return 1;
    }
    
    memset(&modeInfo, 0, sizeof(modeInfo));
    if(vbe_get_modeInfo(submode, &modeInfo)) return 1;
    return 0;
}

int (set_text_mode)() {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = 0x0003;
    reg86.bx = 0x0000;
    if(sys_int86(&reg86) != 0) {
        printf("Set text mode failed.\n");
        return 1;
    }

    memset(&modeInfo, 0, sizeof(modeInfo));
    if(vbe_get_modeInfo(submode, &modeInfo)) return 1;

    return 0;
}

int (set_frameBuffer)(uint16_t mode){
  uint16_t bytes_pp = (modeInfo.BitsPerPixel + 7) >> 3;
  size_t frameSize = modeInfo.XResolution * modeInfo.YResolution * bytes_pp;

  struct minix_mem_range physicAddresses;
  physicAddresses.mr_base = modeInfo.PhysBasePtr;
  physicAddresses.mr_limit = physicAddresses.mr_base + frameSize;
  
  if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physicAddresses)) {
    printf("Physical memory allocation error\n");
    return 1;
  }

  frameBuffer = vm_map_phys(SELF, (void*) physicAddresses.mr_base, frameSize);
  if (frameBuffer == NULL) {
    printf("Virtual memory allocation error\n");
    return 1;
  }

  return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
  if (x > modeInfo.XResolution || y > modeInfo.YResolution) return 1;
  uint16_t bytes_pp = (modeInfo.BitsPerPixel + 7) >> 3;
  size_t i = (modeInfo.XResolution * y + x) * bytes_pp;
  if (memcpy(frameBuffer + i, &color, BytesPerPixel) == NULL) return 1;
  return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
  for (unsigned i = 0 ; i < len ; i++) if (vg_draw_pixel(x + i, y, color)) return 1;
  return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
  for(unsigned i = 0; i < height; i++) if (vg_draw_hline(x, y + i, width, color)) return 1;
  return 0;
}

uint32_t normalize_color(uint32_t color) {
  return color & (BIT(modeInfo.BitsPerPixel) - 1);
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << modeInfo.RedFieldPosition) | (G << modeInfo.GreenFieldPosition) | (B << modeInfo.BlueFieldPosition);
}

uint32_t (R)(uint32_t pixel){
  return ((1 << modeInfo.RedMaskSize) - 1) & (pixel >> modeInfo.RedFieldPosition);
}

uint32_t (G)(uint32_t pixel){
  return ((1 << modeInfo.GreenMaskSize) - 1) & (pixel >> modeInfo.GreenFieldPosition);
}

uint32_t (B)(uint32_t pixel){
  return ((1 << modeInfo.BlueMaskSize) - 1) & (pixel >> modeInfo.BlueFieldPosition);
}