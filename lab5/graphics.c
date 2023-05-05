#include <lcom/lcf.h>
#include "graphics.h"
#include <math.h>

int (set_graphic_mode)(uint16_t submode) {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = VBE_MODE_SET;
    reg86.bx = submode | BIT(14);
    return sys_int86(&reg86);
}

int (set_text_mode)() {
    reg86_t reg86;
    memset(&reg86, 0, sizeof(reg86));
    reg86.intno = 0x10;
    reg86.ax = 0x0003;
    reg86.bx = 0x0000;
    return sys_int86(&reg86);
}

int (set_frame_buffer)(uint16_t mode) {
    memset(&mode_info, 0, sizeof(mode_info));
    if(vbe_get_mode_info(mode, &mode_info)) return 1;

    uint8_t bytes_per_pixel = (mode_info.BitsPerPixel + 7) / 8;
    unsigned int frame_size = mode_info.XResolution * mode_info.YResolution * bytes_per_pixel;
    
    struct minix_mem_range physic_addresses;
    physic_addresses.mr_base = mode_info.PhysBasePtr; // início físico do buffer
    physic_addresses.mr_limit = physic_addresses.mr_base + frame_size; // fim físico do buffer

    if (sys_privctl(SELF, SYS_PRIV_ADD_MEM, &physic_addresses)) return 1;
  
    frame_buffer = vm_map_phys(SELF, (void*) physic_addresses.mr_base, frame_size);
    return (frame_buffer == NULL);
}

int (vg_draw_pixel)(uint16_t x, uint16_t y, uint32_t color) {
    // As coordenadas têm de ser válidas
    if(x >= mode_info.XResolution || y >= mode_info.YResolution) return 1;
    
    // Cálculo dos Bytes per pixel da cor escolhida. Arredondamento por excesso.
    unsigned BytesPerPixel = (mode_info.BitsPerPixel + 7) >> 3;

    // Índice (em bytes) da zona do píxel a colorir
    unsigned int index = (mode_info.XResolution * y + x) * BytesPerPixel;

    // A partir da zona de memória frame_buffer[index], copia @BytesPerPixel bytes da @color
    if (memcpy(frame_buffer + index, &color, BytesPerPixel) == NULL) return 1;

    return 0;
}

int (vg_draw_hline)(uint16_t x, uint16_t y, uint16_t len, uint32_t color) {
    for (int i = 0; i < len; i++) if (vg_draw_pixel(x + i, y, color)) return 1;
    return 0;
}

int (vg_draw_rectangle)(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    for (int i = 0; i < height; i++) if (vg_draw_hline(x, y + i, width, color)) return 1;
    return 0;
}

int (print_xpm)(xpm_map_t xpm, uint16_t x, uint16_t y) {
    xpm_image_t img;

    uint8_t *colors = xpm_load(xpm, XPM_INDEXED, &img);
    if (colors == NULL) return 1;

    for (int h = 0; h < img.height; h++) {
        for (int w = 0; w < img.width; w++) {
            if (vg_draw_pixel(x + w, y + h, *colors) != 0) return 1;
            colors++; // avança para a próxima cor
        }
    }
    return 0;
}

uint32_t (normalize_color)(uint32_t color) {
    if (mode_info.BitsPerPixel == 32) {
        return color;
    } else {
        return color & (BIT(mode_info.BitsPerPixel) - 1);
    }
}

uint32_t (direct_mode)(uint32_t R, uint32_t G, uint32_t B) {
  return (R << mode_info.RedFieldPosition) | (G << mode_info.GreenFieldPosition) | (B << mode_info.BlueFieldPosition);
}

uint32_t (indexed_mode)(uint16_t col, uint16_t row, uint8_t step, uint32_t first, uint8_t n) {
    return (first + (row * n + col) * step) & (BIT(mode_info.BitsPerPixel) - 1);
}

uint32_t (Red)(unsigned j, uint8_t step, uint32_t first) {
    return (R(first) + j * step) & (BIT(mode_info.RedMaskSize) - 1);
}

uint32_t (Green)(unsigned i, uint8_t step, uint32_t first) {
    return (G(first) + i * step) & (BIT(mode_info.GreenMaskSize) - 1);
}

uint32_t (Blue)(unsigned j, unsigned i, uint8_t step, uint32_t first) {
    return (B(first) + (j + i) * step) & (BIT(mode_info.BlueMaskSize) - 1);
}

uint32_t (R)(uint32_t first){
    return (first >> mode_info.RedFieldPosition) & (BIT(mode_info.RedMaskSize) - 1);
}

uint32_t (G)(uint32_t first){
    return (first >> mode_info.GreenFieldPosition) & (BIT(mode_info.GreenMaskSize) - 1);
}

uint32_t (B)(uint32_t first){
    return (first >> mode_info.BlueFieldPosition) & (BIT(mode_info.BlueMaskSize) - 1);
}

uint16_t (getWidth)() {
    return mode_info.XResolution;
}

uint16_t (getHeight)() {
    return mode_info.YResolution;
}
