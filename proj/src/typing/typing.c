
#include "typing.h"

typing_info_t typingInfo = {NULL, NULL, NULL, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 100.0, unready};
racing_info_t racingInfo = {false, false, 0};

void generateText(uint32_t wordAmmount, unsigned seed) {
    srand(seed);
    resetTypingInfo();
    typingInfo.typingCursor = 0;
    typingInfo.generatedText = (char *) malloc(1);
    typingInfo.generatedText[0] = 0;
    typingInfo.generatedTextSize = (wordAmmount == 0) ? 1 : 0;
    for (uint32_t i = 0; i < wordAmmount; i++) {
        float r = ((float) rand()) / RAND_MAX;
        size_t ind = (size_t)(r * dictionarySize);
        char* word = dictionary[ind];
        typingInfo.generatedTextSize += strlen(word) + 1;
        typingInfo.generatedText = realloc(typingInfo.generatedText, typingInfo.generatedTextSize);
        if (i) strcat(typingInfo.generatedText, " ");
        strcat(typingInfo.generatedText, word);
    }
    typingInfo.typedText = (char *) malloc(typingInfo.generatedTextSize);
    memset(typingInfo.typedText, 0, typingInfo.generatedTextSize);
    typingInfo.generatedTextSize--;
    typingInfo.mistakes = (bool *) malloc(typingInfo.generatedTextSize);
    memset(typingInfo.mistakes, false, typingInfo.generatedTextSize);
    typingInfo.status = ready;
}

void resetTypingInfo() {
    free(typingInfo.generatedText);
    free(typingInfo.typedText);
    typingInfo.generatedText = NULL;
    typingInfo.typedText = NULL;
    typingInfo.mistakes = NULL;
    typingInfo.generatedTextSize = 0;
    typingInfo.typingCursor = 0;
    typingInfo.timeStart = 0;
    typingInfo.timeEnd = 0;
    typingInfo.typedChars = 0;
    typingInfo.wrongChars = 0;
    typingInfo.totalWrongChars = 0;
    typingInfo.wpm = 0.0;
    typingInfo.cpm = 0.0;
    typingInfo.acc = 100.0;
    typingInfo.status = unready;
}

void resetRacingInfo() {
    racingInfo.readyMe = false;
    racingInfo.readyYou = false;
    racingInfo.seedMe = 0;
    racingInfo.seedYou = 0;
}

void typingInputHandler() {
    if (typingInfo.status == ready) startTyping();
    if (typingInfo.status == typing) {
        if (scancode == BACKSPACE_SCANCODE) {
            if (typingInfo.typingCursor) {
                typingInfo.typingCursor--;
                typingInfo.typedChars--;
                if (typingInfo.mistakes[typingInfo.typingCursor]) {
                    typingInfo.totalWrongChars--;
                    if (typingInfo.typedText[typingInfo.typingCursor] != typingInfo.generatedText[typingInfo.typingCursor])
                        typingInfo.wrongChars--;
                }
                typingInfo.typedText[typingInfo.typingCursor] = 0;
            }
        } else {
            if (typingInfo.typingCursor < typingInfo.generatedTextSize) {    
                char c = getCharFromMakecode(scancode);
                if (c > 0) {
                    if (typingInfo.mistakes[typingInfo.typingCursor]) {
                        typingInfo.totalWrongChars++;
                    } 
                    if (c != typingInfo.generatedText[typingInfo.typingCursor]) {
                        if (!typingInfo.mistakes[typingInfo.typingCursor]) {
                            typingInfo.mistakes[typingInfo.typingCursor] = true;
                            typingInfo.totalWrongChars++;
                        } 
                        typingInfo.wrongChars++;
                    }
                    typingInfo.typedText[typingInfo.typingCursor] = c;
                    typingInfo.typingCursor++;
                    typingInfo.typedChars++;
                }
                if (typingInfo.typedChars == typingInfo.generatedTextSize && typingInfo.wrongChars == 0) endTyping();
            }
        }
    }
}

void updateTypingInfo() {
    if (typingInfo.status == typing) {
        typingInfo.timeEnd = timer_get_elapsed_count();
        calcTypingStatus();
    }
}
void calcTypingStatus() {
    uint64_t timeDiff = typingInfo.timeEnd - typingInfo.timeStart;
    if (timeDiff == 0) {
        typingInfo.cpm = typingInfo.wpm = 0.0;
        typingInfo.acc = 100.0;
        return;
    }
    double timeDiffSeconds = ((double) timeDiff) / frameRate;
    typingInfo.cpm = (typingInfo.typedChars * 60) / timeDiffSeconds;
    typingInfo.wpm = typingInfo.cpm / 5;
    typingInfo.acc = (100.0 * (typingInfo.typedChars - typingInfo.totalWrongChars)) / typingInfo.typedChars ;
}

void startTyping() {
    typingInfo.status = typing;
    typingInfo.timeStart = timer_get_elapsed_count();
    typingInfo.timeEnd = timer_get_elapsed_count();
}

void endTyping() {
    typingInfo.timeEnd = timer_get_elapsed_count();
    typingInfo.status = finished;
    calcTypingStatus();
}

int drawTypingTest(float cx, float cy, float maxWidth, uint32_t rightColor, uint32_t wrongColor, uint32_t unwrittenColor, font_size_t size) {
    uint16_t x, y, maxW;
    if (maxWidth < 0) {
        x = getXFromPercent(cx) - (getTextWidth(typingInfo.generatedText, size) >> 1);
        maxW = (uint16_t) -1;
    } else {
        x = getXFromPercent(cx) - (getXFromPercent(maxWidth) >> 1);
        maxW = getXFromPercent(maxWidth);
    }
    y = getYFromPercent(cy) - (size.height >> 1);
    uint32_t filteredRightColor = (currentMode == vbe768pInd) ? rightColor : rgbToColor((uint8_t)(rightColor >> 16), (uint8_t)(rightColor >> 8), (uint8_t)rightColor);
    uint32_t filteredWrongColor = (currentMode == vbe768pInd) ? wrongColor : rgbToColor((uint8_t)(wrongColor >> 16), (uint8_t)(wrongColor >> 8), (uint8_t)wrongColor);
    uint32_t filteredUnwrittenColor = (currentMode == vbe768pInd) ? unwrittenColor : rgbToColor((uint8_t)(unwrittenColor >> 16), (uint8_t)(unwrittenColor >> 8), (uint8_t)unwrittenColor);
    char *nextWord = strchr(typingInfo.generatedText, ' ');
    uint16_t dx = 0, dy = 0;
    const uint16_t xStep = size.width + size.pixel;
    const uint16_t yStep = size.height + 3 * size.pixel;
    uint16_t i = 0;
    bool isWrongFromHere = false;
    for (; typingInfo.typedText[i] != 0; i++) {
        if (typingInfo.typingCursor == i) if (vg_draw_rectangle(x + dx, y + dy + size.height + size.pixel, size.width, size.pixel, filteredRightColor)) return 1;
        const char *currChar = typingInfo.generatedText + i;
        if (currChar == nextWord) {
            nextWord = strchr(currChar + 1, ' ');
            uint16_t wordWidth = (nextWord == NULL) ? strlen(currChar) * xStep : (nextWord - currChar) * xStep;
            if (dx + wordWidth > maxW) {
                dx = 0;
                dy += yStep;
                continue;
            }
        }
        if (typingInfo.typedText[i] == typingInfo.generatedText[i]) {
            if (_drawChar(x + dx, y + dy, typingInfo.generatedText[i], filteredRightColor, size)) return 1;  
        } else {
            isWrongFromHere = true;
            if (_drawChar(x + dx, y + dy, typingInfo.generatedText[i], filteredWrongColor, size)) return 1;
        }
        if (isWrongFromHere) if (vg_draw_rectangle(x + dx, y + dy + size.height + size.pixel, size.width, size.pixel, filteredWrongColor)) return 1;
        dx += xStep;
    }
    for (; typingInfo.generatedText[i] != 0; i++) {
        if (typingInfo.typingCursor == i) if (vg_draw_rectangle(x + dx, y + dy + size.height + size.pixel, size.width, size.pixel, filteredRightColor)) return 1;
        const char *currChar = typingInfo.generatedText + i;
        if (currChar == nextWord) {
            nextWord = strchr(currChar + 1, ' ');
            uint16_t wordWidth = (nextWord == NULL) ? strlen(currChar) * xStep : (nextWord - currChar) * xStep;
            if (dx + wordWidth > maxW) {
                dx = 0;
                dy += yStep;
                continue;
            }
        }
        if (_drawChar(x + dx, y + dy, *currChar, filteredUnwrittenColor, size)) return 1;
        dx += xStep;
    }
    if (typingInfo.typingCursor == strlen(typingInfo.generatedText)) if (vg_draw_rectangle(x + dx, y + dy + size.height + size.pixel, size.width, size.pixel, filteredRightColor)) return 1;
    return 0;
}
