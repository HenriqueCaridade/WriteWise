#ifndef _PROJ_UI_H
#define _PROJ_UI_H

#define MAX_AMOUNT_OF_BUTTONS   10
#define CLICK_PX_THRESHOULD     20

#define CURSOR_WIDTH    10
#define CURSOR_HEIGHT   16

#include <stdint.h>
#include <stdbool.h>
#include "../lib/lib.h"
/**
 * @struct button_t
 * @brief This struct holds the properties of each button of our game
 * @var button_t::px 
 * x position in percentage (from 0 to 1) and origin being the upper left corner
 * @var button_t::py
 * y position in percentage (from 0 to 1) and origin being the upper left corner
 * @var button_t::width
 * width of the button in percentage(from 0 to 1)
 * @var button_t::height
 * height of the button in percentage(from 0 to 1)
 * @var button_t::color
 * color in hexadecimal in format RGB(88 88 88)
 * @var button_t::text
 * text that appears in the button
 * @var button_t::tColor
 * color of the text in hexadecimal format RGB(88 88 88)
 * @var button_t::tSize
 * size of the text
 * @var button_t::bColor
 * color of the border in hexadecimal format RGB(88 88 88)
 * @var button_t::onClick
 * function called when button is clicked
 */
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

/**
 * @struct moreMouseInfo
 * @brief Struct that holds the last known mouse information.
 * @var moreMouseInfo::leftPressed
 * true if mouse left button was pressed
 * @var moreMouseInfo::middlePressed
 * true if mouse middle button was pressed
 * @var moreMouseInfo::rightPressed
 * true if mouse right button was pressed
 * @var moreMouseInfo::leftX
 * last cursor x position in pixels when left button was pressed
 * @var moreMouseInfo::leftY when left button was pressed
 * last cursor y position in pixels 
 */
typedef struct {
    bool leftPressed;
    bool middlePressed;
    bool rightPressed;
    uint16_t leftX, leftY;
} moreMouseInfo;

const bool cursorMapMain[CURSOR_HEIGHT][CURSOR_WIDTH];
const bool cursorMapBorder[CURSOR_HEIGHT][CURSOR_WIDTH];

/**
 * @struct cursor
 * @brief Struct that holds cursor information.
 * @var cursor::x
 * x cursor position in pixels
 * @var cursor::y
 * y cursor position in pixels
 * @var cursor::color
 * cursor's color in format RGB(88 88 88)
 * @var cursor::bColor
 * cursor's border color in format RGB(88 88 88)
 * 
 */
typedef struct {
    uint16_t x, y;
    uint32_t color;
    uint32_t bColor;
} cursor;

/**
 * @brief Initializes UI data-structures and allocates static-buffer memory.
 * 
 * @return Non-zero if error occured.
 */
int initUI();

/**
 * @brief Frees static-buffer allocated memory.
 * 
 * @return Non-zero if error occured.
 */
int exitUI();

/**
 * @brief Copies frame-buffer to the static-buffer.
 * 
 * @return Non-zero if error occured.
 */
int calcStaticUI();

/**
 * @brief Copies static-buffer to the frame-buffer.
 * 
 * @return Non-zero if error occured.
 */
int loadStaticUI();

/**
 * @brief Clears the memory used by the buttons.
 * 
 * @return Non-zero if error occured.
 */
int clearButtons();

/**
 * @brief Adds a new button.
 * 
 * @param button 
 * @param index Index of the array where new buttton will be placed.
 * @return Non-zero if error occured.
 */
int addButton(button_t button, int index);

/**
 * @brief Removes button.
 * 
 * @param index Index of the array where the buttton will be removed.
 * @return int 
 */
int removeButton(int index);

/**
 * @brief Draws cursor.
 * 
 * @return Non-zero if error occured.
 */
int drawCursor();
/**
 * @brief Auxiliary function for drawButton and drawButtons.
 * 
 * @param index 
 * @return Non-zero if error occured. 
 */
int _drawButton(int index);

/**
 * @brief Draws button.
 * 
 * @param index 
 * @return Non-zero if error occured. 
 */
int drawButton(int index);

/**
 * @brief Auxiliary function for drawSelectedButton.
 * 
 * @return Non-zero if error occured. 
 */
int _getSelectedButton();

/**
 * @brief Highlights selected button.
 * 
 * @return Non-zero if error occured. 
 */
int drawSelectedButton();
/**
 * @brief Draws multiple buttons.
 * 
 * @return Non-zero if error occured. 
 */
int drawButtons();

/**
 * @brief Checks if mouse clicked on a button.
 * 
 * @param x 
 * X position of the click in pixels.
 * @param y 
 * Y position of the click in pixels.
 * @param button 
 * @return Non-zero if error occured. 
 */
int _isClickOnButton(uint16_t x, uint16_t y, button_t* button);
/**
 * @brief Function that updates mouse every mouse interrupt.
 * 
 * @return Non-zero if error occured. 
 */
int mouseUpdate();

#endif // _PROJ_UI_H
