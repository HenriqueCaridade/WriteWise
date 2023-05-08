
#include "ui.h"

#define CLAMP(x, lower, upper) (MIN(upper, MAX(x, lower)))

button_t _buttons[MAX_AMOUNT_OF_BUTTONS] = {{0, 0, 0, 0, 0, 0, 0, 0, 0, 0}};
bool _buttonPlaces[MAX_AMOUNT_OF_BUTTONS] = {false};

const bool cursorMapMain[CURSOR_HEIGHT][CURSOR_WIDTH] = { // Main Cursor
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 0, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 0, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 0, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 1, 1, 0, 0},
    {0, 1, 1, 1, 1, 1, 0, 0, 0, 0},
    {0, 1, 1, 0, 1, 1, 0, 0, 0, 0},
    {0, 1, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 1, 1, 0, 0, 0},
    {0, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
const bool cursorMapBorder[CURSOR_HEIGHT][CURSOR_WIDTH] = { // Main Cursor
    {1, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 1, 0, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 1, 0, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 1, 0, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 0},
    {1, 0, 0, 0, 0, 0, 0, 0, 1, 1},
    {1, 0, 0, 0, 0, 0, 1, 1, 0, 0},
    {1, 0, 0, 1, 0, 0, 1, 0, 0, 0},
    {1, 0, 1, 0, 1, 0, 0, 1, 0, 0},
    {1, 1, 0, 0, 1, 0, 0, 1, 0, 0},
    {1, 0, 0, 0, 0, 1, 0, 0, 1, 0},
    {0, 0, 0, 0, 0, 1, 1, 1, 1, 0}
};

int initUI() {
    memset(&moreMouseInfo, 0, sizeof(moreMouseInfo));
    cursor.x = cursor.y = 200;
    cursor.color = 0xFFFFFF;
    cursor.bColor = 0x000000;
    uiBuffer = (uint8_t *) malloc(frameSize);
    memset(uiBuffer, 0, frameSize);
    return clearButtons();
}

int exitUI() {
    free(uiBuffer);
    return 0;
}

int calcStaticUI(){
    if (drawButtons()) return 1;
    memcpy(uiBuffer, frameBuffer, frameSize);
    return 0;
}

int loadStaticUI() {
    memcpy(frameBuffer, uiBuffer, frameSize);
    return 0;
}

int clearButtons() {
    memset(_buttons, 0, sizeof(_buttons));
    for (int i = 0; i < MAX_AMOUNT_OF_BUTTONS; i++){
        _buttonPlaces[i] = false;
    }
    return 0;
}

int addButton(button_t button, int index) {
    if (index < 0 || index >= MAX_AMOUNT_OF_BUTTONS) return 1;
    if (_buttonPlaces[index]) return 1;
    _buttons[index] = button;
    _buttonPlaces[index] = true;
    return 0;
}

int removeButton(int index) {
    if (index < 0 || index >= MAX_AMOUNT_OF_BUTTONS) return 1;
    _buttonPlaces[index] = false;
    return 0;
}

int drawCursor() {
    // Ignore if draw outside of the screen
    for (uint16_t dy = 0; dy < CURSOR_HEIGHT; dy++){
        for (uint16_t dx = 0; dx < CURSOR_WIDTH; dx++){
            if (cursorMapBorder[dy][dx]) {
                vg_draw_pixel(cursor.x + dx, cursor.y + dy, cursor.bColor);
            } else if (cursorMapMain[dy][dx]) {
                vg_draw_pixel(cursor.x + dx, cursor.y + dy, cursor.color);
            }
        }
    }
    return 0;
}

int _drawButton(int index){
    button_t *currButton = _buttons + index;
    if (drawRectColor(currButton->x, currButton->y, currButton->width, currButton->height, currButton->color))
        return 1;
    if (currButton->bWidth != 0) {
        // Draw Border
        const uint16_t farX = currButton->x + currButton->width - 1;
        const uint16_t farY = currButton->y + currButton->height - 1;
        for (int i = 0; i < currButton->bWidth; i++) {
            if (drawHLineColor(currButton->x, currButton->y + i, currButton->width, currButton->bColor)) return 1;
            if (drawHLineColor(currButton->x, farY - i, currButton->width, currButton->bColor)) return 1;
            if (drawVLineColor(currButton->x + i, currButton->y, currButton->height, currButton->bColor)) return 1;
            if (drawVLineColor(farX - i, currButton->y, currButton->height, currButton->bColor)) return 1;
        }
    }
    int16_t textWidth = getTextWidth(currButton->text);
    int16_t tx = (int16_t)(currButton->x + (currButton->width  >> 1)) - (textWidth >> 1);
    int16_t ty = (int16_t)(currButton->y + (currButton->height >> 1)) - (currentFontSize.height >> 1);
    if (drawTextColor(tx, ty, currButton->text, currButton->tColor)) return 1;
    return 0;
}
int drawButton(int index){
    if (index < 0 || index >= MAX_AMOUNT_OF_BUTTONS) return 1;
    if (_buttonPlaces[index]) return 1;
    return _drawButton(index);
}

int drawButtons() {
    for (int i = 0; i < MAX_AMOUNT_OF_BUTTONS; i++)
        if (_buttonPlaces[i])
            if(_drawButton(i)) return 1;
    return 0;
}

int _isClickOnButton(uint16_t x, uint16_t y, button_t* button) {
    return (button->x <= x && x < button->x + button->width) &&
           (button->y <= y && y < button->y + button->height);
}

int mouseUpdate() {
    printf("Cursor: %d, %d\n", cursor.x, cursor.y);
    if (!mousePacket.x_ov) cursor.x = CLAMP(cursor.x + mousePacket.delta_x, 0, modeInfo.XResolution);
    if (!mousePacket.y_ov) cursor.y = CLAMP(cursor.y - mousePacket.delta_y, 0, modeInfo.YResolution);
    if (moreMouseInfo.leftPressed && !mousePacket.lb) { // Left Click
        uint16_t dist = abs(moreMouseInfo.leftX - cursor.x) + abs(moreMouseInfo.leftY - cursor.y);
        if (dist <= CLICK_PX_THRESHOULD) { // Valid Click
            for (int i = 0; i < MAX_AMOUNT_OF_BUTTONS; i++) {
                if (_buttonPlaces[i]) {
                    button_t *currButton = _buttons + i;
                    if (_isClickOnButton(moreMouseInfo.leftX, moreMouseInfo.leftY, currButton)) {
                        // Button Click
                        currButton->onClick();
                    }
                }
            }
        }
    }
    /*
    if (moreMouseInfo.middlePressed && !mousePacket.mb) { // Middle Click
    }
    if (moreMouseInfo.rightPressed && !mousePacket.rb) { // Right Click
    }
    */
    if (mousePacket.lb) {
        moreMouseInfo.leftX = cursor.x;
        moreMouseInfo.leftY = cursor.y;
    }
    moreMouseInfo.leftPressed   = mousePacket.lb;
    moreMouseInfo.middlePressed = mousePacket.mb;
    moreMouseInfo.rightPressed  = mousePacket.rb;
    return 0;
}
