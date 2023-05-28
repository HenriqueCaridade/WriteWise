
#ifndef _PROJ_TYPING_H
#define _PROJ_TYPING_H

#include <stdlib.h>
#include <stdint.h>
#include "../lib/lib.h"
#include "../data/data.h"

typedef enum {
    unready,
    ready,
    typing,
    finished,
} typing_status_t;

/**
 * @struct typing_info_t
 * @brief Struct that holds typing information.
 * @var typing_info_t::generatedText
 * Text that was generated which will be typed by the player.
 * @var typing_info_t::typedText
 * Text typed by the player.
 * @var typing_info_t::mistakes
 * Array which points to each character of the text and indicates if the player made a mistake on that letter.
 * @var typing_info_t::generatedTextSize
 * Size of generated text.
 * @var typing_info_t::typingCursor
 * Holds the index of the cursor in the text.
 * @var typing_info_t::timeStart
 * Holds the time of the start of the typing.
 * @var typing_info_t::timeEnd
 * Holds the time of the end of the typing.
 * @var typing_info_t::typedChars
 * Number of typed characters.
 * @var typing_info_t::wrongChars
 * Number of wrongly typed characters in the moment.
 * @var typing_info_t::totalWrongChars
 * Total number of wrongly typed characters.
 * @var typing_info_t::wpm
 * Speed measure of words per minute.
 * @var typing_info_t::cpm
 * Speed measure of characters per minute.
 * @var typing_info_t::acc
 * Accuracy of the typing.
 * @var typing_info_t::status
 * Typing status.
 * 
 */
typedef struct {
    char* generatedText;
    char* typedText;
    bool* mistakes;
    size_t generatedTextSize;
    size_t typingCursor;
    uint64_t timeStart;
    uint64_t timeEnd;
    size_t typedChars;
    size_t wrongChars;
    size_t totalWrongChars;
    double wpm;
    double cpm;
    double acc;
    typing_status_t status;
} typing_info_t;

typing_info_t typingInfo;

/**
 * @struct racing_info_t
 * @brief Struct that holds racing mode info.
 * @var racing_info_t::readyMe
 * If current player is ready to race.
 * @var racing_info_t::readyYou
 * If opponent is ready to race.
 * @var  racing_info_t::seedMe
 * Current player text seed.
 * @var  racing_info_t::seedYou
 * Opponent player text seed.
 */
typedef struct {
    bool readyMe;
    bool readyYou;
    uint64_t seedMe;
    uint64_t seedYou;
    uint64_t startTime;
    bool won;
} racing_info_t;

racing_info_t racingInfo;

/**
 * @brief Generates a text given a word ammount and a creation seed.
 * 
 * This function generates a text by randomly selecting words from a dictionary based on the given word amount and creation seed.
 * @param wordAmmount desired word amount in the generated text.
 * @param seed creation seed used for randomization.
 */
void generateText(uint32_t wordAmmount, unsigned seed);

/**
 * @brief Resets typing info variable.
 * This function resets the typing information variables to their default values. It frees the memory allocated for the generated text and typed text, and sets the corresponding pointers to NULL. It also resets other variables such as mistakes, generated text size, typing cursor, time start and end, typed characters, wrong characters, total wrong characters, words per minute (WPM), characters per minute (CPM), accuracy (ACC), and typing status.
 * 
 */
void resetTypingInfo();
/**
 * @brief Resets racing info variable.
 * 
 * This function resets the racing information variables to their default values. It sets the "readyMe" and "readyYou" flags to false, resets the seed values for both players, sets the start time to zero, and sets the "won" flag to false.
 */
void resetRacingInfo();
/**
 * @brief Handles typing.
 * 
 * This function handles the input during a typing session.
 */
void typingInputHandler(bool isRace);
/**
 * @brief Updates Typing variable.
 * 
 *  This function updates the typing information based on the current typing status.
 * @param isRace true when the typing is for a race, false otherwise. 
 */
void updateTypingInfo();
/**
 * @brief Does the typing status calculations. (WPM, ACC, etc)
 * 
 * This function calculates the typing statistics based on the provided typing information.
 */
void calcTypingStatus();
/**
 * @brief Updates typing status in the beginning of typing.
 * 
 * This function sets the status of the typing information to indicate that typing has started.
 */
void startTyping();
/**
 * @brief Updates typing status in the end of typing.
 * 
 * This function updates the `timeEnd` field of the `typingInfo` structure with the current elapsed time, sets the status of the typing information to indicate that typing has finished, and calculates the typing statistics such as CPM, WPM, and accuracy by calling the `calcTypingStatus` function.
 */
void endTyping();
/**
 * @brief Sends INFO_WON code through the serial port.
 * 
 * This function sets the `won` flag in the `racingInfo` structure to true and sends the INFO_WON code through the serial port by calling the `serialPortSendWon` function.
 */
void endRace();

/**
 * @brief Draws the text which the player must write.
 * 
 * This function draws the text that the player needs to type on the screen. The text is positioned based on the provided center coordinates (cx, cy) as a percentage of the screen width and height.
 * @param cx x coordinate in percentage(0 to 1) starting from the center. 
 * @param cy y coordinate in percentage(0 to 1) starting from the center. 
 * @param maxWidth text max width.
 * @param rightColor color of the text when the characters are correctly written.
 * @param wrongColor color of the text when the characters are incorrectly written.
 * @param unwrittenColor color of unwritten text.
 * @param size font size
 * @return Non-zero if error occured. 
 */
int drawTypingTest(float cx, float cy, float maxWidth, uint32_t rightColor, uint32_t wrongColor, uint32_t unwrittenColor, font_size_t size);

#endif // _PROJ_TYPING_H
