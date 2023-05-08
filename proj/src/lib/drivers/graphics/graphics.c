#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>

int (set_graphic_mode)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = 0x4F02;
    reg86.bx = submode | BIT(14);
    if (sys_int86(&reg86) != OK) {
        printf("Set graphic mode failed.\n");
        return 1;
    }
    
    memset(&modeInfo, 0, sizeof(modeInfo));
    if(vbe_get_mode_info(submode, &modeInfo)) {
        printf("vbe_get_mode_info failed.");
        return 1;
    }
    bytes_pp = (modeInfo.BitsPerPixel + 7) >> 3;
    frameSize = modeInfo.XResolution * modeInfo.YResolution * bytes_pp;
    return 0;
}

int (set_text_mode)() {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = 0x0003;
    if(sys_int86(&reg86) != OK) {
        printf("Set text mode failed.\n");
        return 1;
    }

    memset(&modeInfo, 0, sizeof(modeInfo));
    return 0;
}

int (set_frame_buffer)(){
    // ONLY CALLED AFTER set_graphic_mode!!
    struct minix_mem_range physicAddresses;
    physicAddresses.mr_base = modeInfo.PhysBasePtr;
    physicAddresses.mr_limit = physicAddresses.mr_base + frameSize;
    
    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physicAddresses)) {
        printf("Physical memory allocation error.\n");
        return 1;
    }

    frameBuffer = vm_map_phys(SELF, (void*) physicAddresses.mr_base, frameSize);
    if (frameBuffer == NULL) {
        printf("Virtual memory allocation error.\n");
        return 1;
    }

    return 0;
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    if (x > modeInfo.XResolution || y > modeInfo.YResolution) {
        printf("Pixel Coords (%d, %d) out of bounds(%d, %d).\n", x, y, modeInfo.XResolution, modeInfo.YResolution);
        return 1;
    }
    size_t i = (modeInfo.XResolution * y + x) * bytes_pp;
    memcpy(frameBuffer + i, &color, bytes_pp);
    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (unsigned i = 0 ; i < len ; i++) if (vg_draw_pixel(x + i, y, color)) return 1;
    return 0;
}

int (vg_draw_vline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (unsigned i = 0 ; i < len ; i++) if (vg_draw_pixel(x, y + i, color)) return 1;
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for(unsigned i = 0; i < height; i++) if (vg_draw_hline(x, y + i, width, color)) return 1;
    return 0;
}

uint32_t (direct_mode)(uint8_t red, uint8_t green, uint8_t blue) {
    return ((uint32_t)(red   >> (8 - modeInfo.RedMaskSize))   << modeInfo.RedFieldPosition) |
           ((uint32_t)(green >> (8 - modeInfo.GreenMaskSize)) << modeInfo.GreenFieldPosition) |
           ((uint32_t)(blue  >> (8 - modeInfo.BlueMaskSize))  << modeInfo.BlueFieldPosition);
}
