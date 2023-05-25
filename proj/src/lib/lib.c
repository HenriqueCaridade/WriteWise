
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
    printf("Set %d mode successfully!\n", currentMode);
    return 0;
}

int exitGraphMode(int code) {
    int attempts = 10;
    while (attempts-- && exit_graphic_mode()) tickdelay(micros_to_ticks(20000));;
    return code;
}

int setTheme(app_theme_t newTheme) {
    if (newTheme != darkTheme && newTheme != lightTheme) return 1;
    currentTheme = newTheme;
    return 0;
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
    switch (currentTheme){
    case darkTheme:  memset(frameBuffer, 0x00, frameSize); return;
    case lightTheme: memset(frameBuffer, 0xff, frameSize); return;
    }
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

int _drawRect(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint32_t color) {
    return vg_draw_rectangle(x, y, width, height, color);
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

int _drawText(uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size){
    // Only called when color is already calculated
    char* nextWord = strchr(str, ' ');
    uint16_t dx = 0, dy = 0;
    const uint16_t xStep = size.width + size.pixel;
    const uint16_t yStep = size.height + size.pixel;
    for (; *str != 0; str++) {
        if (str == nextWord) {
            nextWord = strchr(str + 1, ' ');
            uint16_t wordWidth = (nextWord == NULL) ? strlen(str) * xStep : (nextWord - str) * xStep;
            if (dx + wordWidth > maxWidth) {
                dx = 0;
                dy += yStep;
                continue;
            }
        }
        if (_drawChar(x + dx, y + dy, *str, color, size)) return 1;
        dx += xStep;
    }
    return 0;
}
int drawTextColor(float cx, float cy, float maxWidth, const char* str, uint32_t color, font_size_t size) {
    uint16_t x, y, maxW;
    if (maxWidth < 0) {
        x = getXFromPercent(cx) - (getTextWidth(str, size) >> 1);
        maxW = (uint16_t) -1;
    } else {
        x = getXFromPercent(cx) - (getXFromPercent(maxWidth) >> 1);
        maxW = getXFromPercent(maxWidth);
    }
    y = getYFromPercent(cy) - (size.height >> 1);
    if (currentMode == vbe768pInd) return _drawText(x, y, maxW, str, color, size);
    return _drawText(x, y, maxW, str, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color), size);
}
int drawTextRGB(float cx, float cy, float maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size){
    uint16_t x, y, maxW;
    if (maxWidth < 0) {
        x = getXFromPercent(cx) - (getTextWidth(str, size) >> 1);
        maxW = (uint16_t) -1;
    } else {
        x = getXFromPercent(cx) - (getXFromPercent(maxWidth) >> 1);
        maxW = getXFromPercent(maxWidth);
    }
    y = getYFromPercent(cy) - (size.height >> 1);
    if (currentMode == vbe768pInd) { printf("drawTextRGB function called when in Indexed Mode.\n"); return 1; }
    return _drawText(x, y, maxW, str, direct_mode(red, green, blue), size);
}
int drawTextXYColor(uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint32_t color, font_size_t size) {
    if (currentMode == vbe768pInd) return _drawText(x, y, maxWidth, str, color, size);
    return _drawText(x, y, maxWidth, str, direct_mode((uint8_t)(color >> 16), (uint8_t)(color >> 8), (uint8_t)color), size);
}
int drawTextXYRGB(uint16_t x, uint16_t y, uint16_t maxWidth, const char* str, uint8_t red, uint8_t green, uint8_t blue, font_size_t size){
    if (currentMode == vbe768pInd) { printf("drawTextRGB function called when in Indexed Mode.\n"); return 1; }
    return _drawText(x, y, maxWidth, str, direct_mode(red, green, blue), size);
}
uint16_t getTextWidth(const char* str, font_size_t size) {
    return strlen(str) * (size.width + size.pixel) - size.pixel;
}
