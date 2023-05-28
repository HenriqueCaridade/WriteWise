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
 * color in hexadecimal in format RGB (888)
 * @var button_t::text
 * text that appears in the button
 * @var button_t::tColor
 * color of the text in hexadecimal format RGB(888)
 * @var button_t::tSize
 * size of the text
 * @var button_t::bColor
 * color of the border in hexadecimal format RGB(888)
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
 * @struct mouse_info_t
 * @brief Struct that holds the last known mouse information.
 * @var mouse_info_t::leftPressed
 * true if mouse left button was pressed
 * @var mouse_info_t::middlePressed
 * true if mouse middle button was pressed
 * @var mouse_info_t::rightPressed
 * true if mouse right button was pressed
 * @var mouse_info_t::leftX
 * last cursor x position in pixels when left button was pressed
 * @var mouse_info_t::leftY when left button was pressed
 * last cursor y position in pixels 
 */
typedef struct {
    bool leftPressed;
    bool middlePressed;
    bool rightPressed;
    uint16_t leftX, leftY;
} mouse_info_t;
mouse_info_t moreMouseInfo;

const bool cursorMapMain[CURSOR_HEIGHT][CURSOR_WIDTH];
const bool cursorMapBorder[CURSOR_HEIGHT][CURSOR_WIDTH];

/**
 * @struct cursor_t
 * @brief Struct that holds cursor information.
 * @var cursor_t::x
 * x cursor position in pixels
 * @var cursor_t::y
 * y cursor position in pixels
 * @var cursor_t::color
 * cursor's color in format RGB(888)
 * @var cursor_t::bColor
 * cursor's border color in format RGB(888)
 * 
 */
typedef struct {
    uint16_t x, y;
    uint32_t color;
    uint32_t bColor;
} cursor_t;
cursor_t cursor;

/**
 * @brief Initializes UI data-structures and allocates static-buffer memory.
 * 
 * This function initializes the UI data structures and sets the initial values for the cursor position, color, and background color.
 * @return Non-zero if error occured.
 */
int initUI();

/**
 * @brief Frees static-buffer allocated memory.
 * 
 * This function frees the memory allocated for the static buffer using the `free` function.
 * @return Non-zero if error occured.
 */
int exitUI();

/**
 * @brief Copies frame-buffer to the static-buffer.
 * 
 * This function copies the content of the frame buffer to the static buffer using the `memcpy` function.
 * @return Non-zero if error occured.
 */
int calcStaticUI();

/**
 * @brief Copies static-buffer to the frame-buffer.
 * 
 * This function copies the content of the static buffer to the frame buffer using the `memcpy` function.
 * @return Non-zero if error occured.
 */
int loadStaticUI();

/**
 * @brief Clears the memory used by the buttons.
 * 
 * This function clears the memory used by the buttons by setting the `_buttons` array to all zeros using the `memset` function.
 * @return Non-zero if error occured.
 */
int clearButtons();

/**
 * @brief Adds a new button.
 * 
 * This function adds a new button to the button array at the specified index.
 * @param button button to be added.
 * @param index Index of the array where new buttton will be placed.
 * @return Non-zero if error occured.
 */
int addButton(button_t button, int index);

/**
 * @brief Removes button.
 * 
 * This function removes a button at the specified index from the button array.
 * @param index index of the button to be removed.
 * @return int 
 */
int removeButton(int index);

/**
 * @brief Draws cursor.
 * 
 * This function draws the cursor on the screen based on the current cursor position.
 * @return Non-zero if error occured.
 */
int drawCursor();
/**
 * @brief Auxiliary function for drawButton and drawButtons.
 * 
 * This is an auxiliary function used by the `drawButton` and `drawButtons` functions.
 * @param index index of the button to be drawn.
 * @return Non-zero if error occured. 
 */
int _drawButton(int index);

/**
 * @brief Draws button.
 * 
 * This function draws a button based on the provided index.
 * @param index index of the button to be drawn.
 * @return Non-zero if error occured. 
 */
int drawButton(int index);

/**
 * @brief Auxiliary function for drawSelectedButton.
 * 
 * This function iterates through the available buttons and checks if the mouse cursor is positioned over each button using the _isClickOnButton function.
 * @return Non-zero if error occured. 
 */
int _getSelectedButton();

/**
 * @brief Highlights selected button.
 * 
 * This function retrieves the index of the selected button using the _getSelectedButton function.
 * @return Non-zero if error occured. 
 */
int drawSelectedButton();
/**
 * @brief Draws multiple buttons.
 * 
 * This function iterates over the available buttons and draws each one that is active.
 * @return Non-zero if error occured. 
 */
int drawButtons();

/**
 * @brief Checks if mouse clicked on a button.
 * 
 * This function checks if a mouse click event occurred at the specified position (x, y) and determines if it falls within the boundaries of the given button.
 * @param x x position of the click in pixels.
 * @param y y position of the click in pixels.
 * @param button pointer to the button structure to check against.
 * @return Non-zero if error occured. 
 */
int _isClickOnButton(uint16_t x, uint16_t y, button_t* button);
/**
 * @brief Function that updates mouse every mouse interrupt.
 * 
 * It reads the mouse packet and performs the necessary calculations and actions based on the mouse input.
 * @return Non-zero if error occured. 
 */
int mouseUpdate();

#endif // _PROJ_UI_H
