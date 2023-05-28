#ifndef _PROJ_UI_H
#define _PROJ_UI_H

#define MAX_AMOUNT_OF_BUTTONS   10
#define CLICK_PX_THRESHOULD     20

#define CURSOR_WIDTH    10
#define CURSOR_HEIGHT   16

#include <stdint.h>
#include <stdbool.h>
#include "../lib/lib.h"

typedef struct {
    float px;               // X pos
    float py;               // Y pos
    float width;            // Width
    float height;           // Height
    uint32_t color;         // Background Color
    char *text;             // Button Text
    uint32_t tColor;        // Text Color
    font_size_t tSize;      // Text Size
    uint32_t bColor;        // Border Color
    void (*onClick)(void);  // On Click void callback
} button_t;

button_t _buttons[MAX_AMOUNT_OF_BUTTONS];
bool _buttonPlaces[MAX_AMOUNT_OF_BUTTONS];
uint8_t *staticBuffer;

struct {
    bool leftPressed;
    bool middlePressed;
    bool rightPressed;
    uint16_t leftX, leftY;
} moreMouseInfo;

const bool cursorMapMain[CURSOR_HEIGHT][CURSOR_WIDTH];
const bool cursorMapBorder[CURSOR_HEIGHT][CURSOR_WIDTH];

struct {
    uint16_t x, y;
    uint32_t color;
    uint32_t bColor;
} cursor;

int initUI();
int exitUI();

int calcStaticUI();
int loadStaticUI();

int clearButtons();
int addButton(button_t button, int index);
int removeButton(int index);

int drawCursor();
int _drawButton(int index);
int drawButton(int index);
int _getSelectedButton();
int drawSelectedButton();
int drawButtons();

int _isClickOnButton(uint16_t x, uint16_t y, button_t* button);
int mouseUpdate();

#endif // _PROJ_UI_H
