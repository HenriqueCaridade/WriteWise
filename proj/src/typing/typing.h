
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

typedef struct {
    bool readyMe;
    bool readyYou;
    uint64_t seedMe;
    uint64_t seedYou;
} racing_info_t;
racing_info_t racingInfo;

void generateText(uint32_t wordAmmount, unsigned seed);
void typingInputHandler();
void updateTypingInfo();
void resetTypingInfo();
void startTyping();
void endTyping();

int drawTypingTest(float cx, float cy, float maxWidth, uint32_t rightColor, uint32_t wrongColor, uint32_t unwrittenColor, font_size_t size);

#endif // _PROJ_TYPING_H
