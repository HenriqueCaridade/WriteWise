#include "lib.h"


int setFrameRate(uint16_t fps){
    if (fps < 20 || fps > 60) {
        printf("Frame Rate must be set between 20 and 60.\n");
        return 0;
    }
    return timer_set_frequency(0, fps);
}

int setMinixMode(minix_mode_t mode){
    if (mode == textMode){
        if (set_text_mode()) return 1;
        currentMode = mode;
    } else {
        if (set_graphic_mode(mode)) return 1;
        if (set_frame_buffer()) { setMinixMode(textMode); return 1;}
        currentMode = mode;
    }
    return 0;
}

void clearScreen(){
    memset(frameBuffer, 0, frameSize);
}

int drawPixelColor(uint16_t x, uint16_t y, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_pixel(x, y, color);
    return vg_draw_pixel(x, y, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawPixelRGB(uint16_t x, uint16_t y, uint8_t red, uint8_t green, uint8_t blue) {
    if (currentMode == vbe768pInd) { printf("drawPixelRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_pixel(x, y, direct_mode(red, green, blue));
}

int drawHLineColor(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_hline(x, y, len, color);
    return vg_draw_hline(x, y, len, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawHLineRGB(uint16_t x, uint16_t y, uint16_t len, uint8_t red, uint8_t green, uint8_t blue) {
    if (currentMode == vbe768pInd) { printf("drawHLineRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_hline(x, y, len, direct_mode(red, green, blue));
}

int drawVLineColor(uint16_t x, uint16_t y, uint16_t len, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_vline(x, y, len, color);
    return vg_draw_vline(x, y, len, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawVLineRGB(uint16_t x, uint16_t y, uint16_t len, uint8_t red, uint8_t green, uint8_t blue){
    if (currentMode == vbe768pInd) { printf("drawVLineRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_vline(x, y, len, direct_mode(red, green, blue));
}

int drawRectColor(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_rectangle(x, y, width, height, color);
    return vg_draw_rectangle(x, y, width, height, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawRectRGB(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint8_t red, uint8_t green, uint8_t blue) {
    if (currentMode == vbe768pInd) { printf("drawRectRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_rectangle(x, y, width, height, direct_mode(red, green, blue));
}

int _drawChar(uint16_t x, uint16_t y, const char c, uint32_t color){
    // Only called when color is already calculated
    int index = getCharIndex(c);
    if (index < 0) return 1;
    for (uint16_t dy = 0; dy < currentFontSize.height; dy++){
        for (uint16_t dx = 0; dx < currentFontSize.width; dx++){
            if (alphabet[index][dy / currentPixelSize][dx / currentPixelSize]) {
                if (vg_draw_pixel(x + dx, y + dy, color)) return 1;
            }
        }
    }
    return 0;
}
int drawCharColor(uint16_t x, uint16_t y, const char c, uint32_t color){
    if (currentMode == vbe768pInd) return _drawChar(x, y, c, color);
    return _drawChar(x, y, c, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawCharRGB(uint16_t x, uint16_t y, const char c, uint8_t red, uint8_t green, uint8_t blue){
    if (currentMode == vbe768pInd) { printf("drawCharRGB function called when in Indexed Mode.\n"); return 1; }
    return _drawChar(x, y, c, direct_mode(red, green, blue));
}

int _drawText(uint16_t x, uint16_t y, const char* str, uint32_t color){
    // Only called when color is already calculated
    for (int i = 0; str[i] != 0; i++){
        uint16_t dx = i * (currentFontSize.width + currentPixelSize);
        if (_drawChar(x + dx, y, str[i], color)) return 1;
    }
    return 0;
}
int drawTextColor(uint16_t x, uint16_t y, const char* str, uint32_t color){
    if (currentMode == vbe768pInd) return _drawText(x, y, str, color);
    return _drawText(x, y, str, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawTextRGB(uint16_t x, uint16_t y, const char* str, uint8_t red, uint8_t green, uint8_t blue){
    if (currentMode == vbe768pInd) { printf("drawTextRGB function called when in Indexed Mode.\n"); return 1; }
    return _drawText(x, y, str, direct_mode(red, green, blue));
}

int initAll(){
    isScancodeTwoBytes = false;
    currentMode = textMode;
    setFontType(medium);
    if (timer_subscribe_interrupt()) return 1;
    if (keyboard_subscribe_int()) return 1;
    if (mouse_subscribe_int()) return 1;
    return 0;
}

int exitAll(){
    if (mouse_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe_int()) return 1;
    if (timer_unsubscribe_int()) return 1;
    return 0;
}
