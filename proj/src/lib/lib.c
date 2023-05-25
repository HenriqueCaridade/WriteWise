
#include "lib.h"

int initAllDrivers(){
    isScancodeTwoBytes = false;
    currentMode = textMode;
    if (timer_subscribe_interrupt()) return 1;
    if (keyboard_subscribe_int()) return 1;
    if (mouse_subscribe_int()) return 1;
    return 0;
}

int exitAllDrivers(){
    if (mouse_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe_int()) return 1;
    if (timer_unsubscribe_int()) return 1;
    return 0;
}

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
        if (set_frame_buffer()) { setMinixMode(textMode); return 1; }
        currentMode = mode;
    }
    return 0;
}

int exitGraphMode(int code) {
    int attempts = 10;
    while (attempts-- && set_text_mode());
    return code;
}

uint16_t getXFromPercent(float px) {
    switch (currentMode) {
        case vbe480pDc:
            return (uint16_t) (px * 640);
        case vbe600pDc:
            return (uint16_t) (px * 800);
        case vbe768pInd:
            return (uint16_t) (px * 1024);
        case vbe864pDc:
            return (uint16_t) (px * 1152);
        case vbe1024pDc:
            return (uint16_t) (px * 1280);
        default:
            return 0;
    }
}
uint16_t getYFromPercent(float py) {
    switch (currentMode) {
        case vbe480pDc:
            return (uint16_t) (py * 480);
        case vbe600pDc:
            return (uint16_t) (py * 600);
        case vbe768pInd:
            return (uint16_t) (py * 768);
        case vbe864pDc:
            return (uint16_t) (py * 864);
        case vbe1024pDc:
            return (uint16_t) (py * 1024);
        default:
            return 0;
    }
}


void clearScreen(){
    memset(frameBuffer, 0, frameSize);
}

int drawPixelColor(float px, float py, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_pixel(getXFromPercent(px), getYFromPercent(py), color);
    return vg_draw_pixel(getXFromPercent(px), getYFromPercent(py), direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawPixelRGB(float px, float py, uint8_t red, uint8_t green, uint8_t blue) {
    if (currentMode == vbe768pInd) { printf("drawPixelRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_pixel(getXFromPercent(px), getYFromPercent(py), direct_mode(red, green, blue));
}

int drawHLineColor(float px, float py, float len, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_hline(getXFromPercent(px), getYFromPercent(py), getXFromPercent(len), color);
    return vg_draw_hline(getXFromPercent(px), getYFromPercent(py), getXFromPercent(len) , direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawHLineRGB(float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue) {
    if (currentMode == vbe768pInd) { printf("drawHLineRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_hline(getXFromPercent(px), getYFromPercent(py), getXFromPercent(len), direct_mode(red, green, blue));
}

int drawVLineColor(float px, float py, float len, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_vline(getXFromPercent(px), getYFromPercent(py), getYFromPercent(len), color);
    return vg_draw_vline(getXFromPercent(px), getYFromPercent(py), getYFromPercent(len), direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawVLineRGB(float px, float py, float len, uint8_t red, uint8_t green, uint8_t blue){
    if (currentMode == vbe768pInd) { printf("drawVLineRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_vline(getXFromPercent(px), getYFromPercent(py), getYFromPercent(len), direct_mode(red, green, blue));
}

int drawRectColor(float px, float py, float width, float height, uint32_t color){
    if (currentMode == vbe768pInd) return vg_draw_rectangle(getXFromPercent(px), getYFromPercent(py), getXFromPercent(width), getYFromPercent(height), color);
    return vg_draw_rectangle(getXFromPercent(px), getYFromPercent(py), getXFromPercent(width), getYFromPercent(height), direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color));
}
int drawRectRGB(float px, float py, float width, float height, uint8_t red, uint8_t green, uint8_t blue) {
    if (currentMode == vbe768pInd) { printf("drawRectRGB function called when in Indexed Mode.\n"); return 1; }
    return vg_draw_rectangle(getXFromPercent(px), getYFromPercent(py), getXFromPercent(width), getYFromPercent(height), direct_mode(red, green, blue));
}

int _draw5x7(uint16_t x, uint16_t y, const bool c[7][5], uint32_t color, font_size_t size) {
    for (uint16_t dy = 0; dy < size.height; dy++){
        for (uint16_t dx = 0; dx < size.width; dx++){
            if (c[dy / size.pixel][dx / size.pixel]) {
                if (vg_draw_pixel(x + dx, y + dy, color)) return 1;
            }
        }
    }
    return 0;
}
int _drawChar(uint16_t x, uint16_t y, const char c, uint32_t color, font_size_t size){
    // Only called when color is already calculated
    int index = getCharIndex(c);
    if (index < 0) return 1;
    return _draw5x7(x, y, alphabet[index], color, size);
}
int drawCharColor(float px, float py, const char c, uint32_t color, font_size_t size){
    if (currentMode == vbe768pInd) return _drawChar(getXFromPercent(px), getYFromPercent(py), c, color, size);
    return _drawChar(getXFromPercent(px), getYFromPercent(py), c, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color), size);
}
int drawCharRGB(float px, float py, const char c, uint8_t red, uint8_t green, uint8_t blue, font_size_t size){
    if (currentMode == vbe768pInd) { printf("drawCharRGB function called when in Indexed Mode.\n"); return 1; }
    return _drawChar(getXFromPercent(px), getYFromPercent(py), c, direct_mode(red, green, blue), size);
}

int _drawText(uint16_t x, uint16_t y, const char* str, uint32_t color, font_size_t size){
    // Only called when color is already calculated
    for (int i = 0; str[i] != 0; i++){
        uint16_t dx = i * (size.width + size.pixel);
        if (_drawChar(x + dx, y, str[i], color, size)) return 1;
    }
    return 0;
}
int drawTextColor(float cx, float cy, const char* str, uint32_t color, font_size_t size) {
    uint16_t x = getXFromPercent(cx) - (getTextWidth(str, size) >> 1);
    uint16_t y = getYFromPercent(cy) - (size.height >> 1);
    if (currentMode == vbe768pInd) return _drawText(x, y, str, color, size);
    return _drawText(x, y, str, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color), size);
}
int drawTextRGB(float cx, float cy, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size){
    uint16_t x = getXFromPercent(cx) - (getTextWidth(str, size) >> 1);
    uint16_t y = getXFromPercent(cy) - (size.height >> 1);
    if (currentMode == vbe768pInd) { printf("drawTextRGB function called when in Indexed Mode.\n"); return 1; }
    return _drawText(x, y, str, direct_mode(red, green, blue), size);
}
uint16_t getTextWidth(const char* str, font_size_t size) {
    return strlen(str) * (size.width + size.pixel) - size.pixel;
}
