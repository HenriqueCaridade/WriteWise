// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "lib/lib.h"
#include "ui/ui.h"
#include "data/data.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US");

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/proj/src/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

typedef enum {
    startState,
    mainState,
    trainingState,
    raceState,
    instructionsState,
    settingsState,
    endState,
} app_state_t;
app_state_t currAppState = startState;

typedef enum {
    textColor = 0,
    subtleColor,
    buttonBackColor,
    buttonTextColor,
    selectedButtonColor,
    trainingColor,
    trainingSubtleColor,
    raceColor,
    raceSubtleColor,
} theme_color_index_t;

uint32_t darkThemeColorArray[] = {
    0xFFFFFF,
    0x505050,
    0x303030,
    0xDDDDDD,
    0x606060,
    0x4ecfc4,
    0xa2e0db,
    0xa670e0,
    0xb79ad6,
};

uint32_t lightThemeColorArray[] = {
    0x000000,
    0xA0A0A0,
    0xC0C0C0,
    0x222222,
    0x909090,
    0x4ecfc4,
    0xa2e0db,
    0xa670e0,
    0xb79ad6,
};

uint32_t getThemeColor(theme_color_index_t color) {
    if (currentTheme == darkTheme) return darkThemeColorArray[color];
    else return lightThemeColorArray[color];
}

int drawScreen();
int loadScreen();
int changeState();
void keyboardScancodeHandler();
char* generatedText = NULL;
char* typedText = NULL;
size_t generatedTextSize = 0;
size_t typingCursor = 0;
void generateText(uint32_t wordAmmount);

int exitAll(int code) {
    free(generatedText);
    free(typedText);
    freeDictionary();
    exitGraphMode(code);
    exitAllDrivers();
    return code;
}

int(proj_main_loop)(int argc, char *argv[]){
    if (initAllDrivers()) return 1;
    if (setFrameRate(30)) { exitAllDrivers(); return 1; }
    if (setMinixMode(vbe600pDc)) { exitAllDrivers(); return 1; }
    if (initUI() || loadScreen() || setTheme(darkTheme)) {
        printf("Failed Initializing UI.\n");
        return exitAll(1);
    }
    if (loadDictionary("/home/lcom/labs/proj/src/data/english.dict")) {
        printf("Failed Initializing Dictionary.\n");
        return exitAll(1);
    }
    while (currAppState != endState) {
        if (driver_receive(ANY, &msg, &ipcStatus)) {
            printf("driver_receive failed.");
            continue;
        }
        if (is_ipc_notify(ipcStatus)) {
        switch (_ENDPOINT_P(msg.m_source)) {
            case HARDWARE:
            if (msg.m_notify.interrupts & irqSetTimer) {
                // Timer Interrupt
                timer_int_handler();
                // Draw screen
                if (drawScreen(currAppState)) return exitAll(1);
            }
            if (msg.m_notify.interrupts & irqSetMouse) {
                // Mouse Interrupt
                if (mouse_int_h()) {
                    // Complete Packet
                    mouseUpdate();
                }
            }
            if (msg.m_notify.interrupts & irqSetKeyboard) {
                // Keyboard Interrupt
                if (keyboard_ih()) {
                    // New Scancode
                    keyboardScancodeHandler();
                }
            }
            break;
        }
        }
    }
    return exitAll(0);
}
int changeState(app_state_t newState){
    printf("Changed From %d to %d.\n", currAppState, newState);
    currAppState = newState;
    switch (newState) {
    case endState: return 0;
    case trainingState: generateText(25); break;
    default: break;
    }
    return loadScreen();
}

void keyboardScancodeHandler() {
    if (scancode & BREAK_CODE) {
        // BREAK CODE
        if (!isScancodeTwoBytes && scancode == BREAK_ESC) {
            switch(currAppState) {
                case mainState: changeState(endState); break;
                default: changeState(mainState); break;
            }
            return;
        }
    } else {
        // MAKE CODE
        if (!isScancodeTwoBytes && scancode == MAKE_ESC) return;
        if (currAppState == startState) {
            changeState(mainState);
            return;
        }
        if (currAppState == trainingState) {
            if (scancode == BACKSPACE_SCANCODE) {
                if (typingCursor) typedText[--typingCursor] = 0;
            } else {
                char c = getCharFromMakecode(scancode);
                if (c > 0) typedText[typingCursor++] = c;
                if (generatedTextSize && typingCursor >= generatedTextSize) typingCursor--;
            }
        }
        if (isScancodeTwoBytes) {
            switch (scancode) {
                default: return;
            }
        } else {
            switch (scancode) {
                default: return;
            }
        }
    }
}

int startScreenLoad() {
    // TEST UI
    clearScreen();
    clearButtons();
    if (drawTextColor(0.5f, 0.30f, -1.0f, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press any key to start...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    return calcStaticUI();
}

void _mainButtonTraining    (void) { changeState(trainingState);     }
void _mainButtonRace        (void) { changeState(raceState);         }
void _mainButtonInstructions(void) { changeState(instructionsState); }
void _mainButtonSettings    (void) { changeState(settingsState);     }
int mainScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Main Started Drawing.\n");
    if (drawTextColor(0.5f, 0.15f, -1.0f, "Write Wise", getThemeColor(textColor), getFontSize(xxlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to exit...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (addButton((button_t){0.3f, 0.30f, 0.4f, 0.1f, getThemeColor(buttonBackColor), "Training"    , getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _mainButtonTraining    }, 0)) return 1;
    if (addButton((button_t){0.3f, 0.45f, 0.4f, 0.1f, getThemeColor(buttonBackColor), "Race"        , getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _mainButtonRace        }, 1)) return 1;
    if (addButton((button_t){0.3f, 0.60f, 0.4f, 0.1f, getThemeColor(buttonBackColor), "Instructions", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _mainButtonInstructions}, 2)) return 1;
    if (addButton((button_t){0.3f, 0.75f, 0.4f, 0.1f, getThemeColor(buttonBackColor), "Settings"    , getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _mainButtonSettings    }, 3)) return 1;
    printf("Main Screen Drawn.\n");
    return calcStaticUI();
}

int trainingScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Traning Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xlarge).height, -1, "Training", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.4f, 0.8f, generatedText, getThemeColor(subtleColor), getFontSize(medium))) return 1;
    printf("Traning Screen Drawn.\n");
    return calcStaticUI();
}
int raceScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Race Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xlarge).height, -1, "Race", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    printf("Race Screen Drawn.\n");
    return calcStaticUI();
}
int instructionScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Instructions Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xlarge).height, -1, "Instructions", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.2f, -1.0f, "Training", getThemeColor(trainingColor), getFontSize(large))) return 1;
    if (drawTextColor(0.5f, 0.3f,  0.8f,
        "In the Training section you can train your typing speed before you race others. It provides a safe environment where you can practice without worring about others.",
        getThemeColor(trainingSubtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.5f, -1.0f, "Race", getThemeColor(raceColor), getFontSize(large))) return 1;
    if (drawTextColor(0.5f, 0.6f,  0.8f,
        "In the Race section you will race an opponent and the first to finish typing the given words will win!",
        getThemeColor(raceSubtleColor), getFontSize(medium))) return 1;
    printf("Instructions Screen Drawn.\n");
    return calcStaticUI();
}

int settingsScreenLoad();
void _settingsButton480p      (void) { exitUI(); setMinixMode(vbe480pDc);  initUI(); settingsScreenLoad(); }
void _settingsButton600p      (void) { exitUI(); setMinixMode(vbe600pDc);  initUI(); settingsScreenLoad(); }
void _settingsButton864p      (void) { exitUI(); setMinixMode(vbe864pDc);  initUI(); settingsScreenLoad(); }
void _settingsButton1024p     (void) { exitUI(); setMinixMode(vbe1024pDc); initUI(); settingsScreenLoad(); }
void _settingsButtonDarkTheme (void) { setTheme(darkTheme);  settingsScreenLoad(); }
void _settingsButtonLightTheme(void) { setTheme(lightTheme); settingsScreenLoad(); }
int settingsScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Settings Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xlarge).height, -1, "Settings", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawTextColor(0.25f, 0.15f, -1.0f, "Resolution", getThemeColor(textColor), getFontSize(medium))) return 1;
    if (addButton((button_t){0.1f, 0.30f, 0.3f, 0.1f, getThemeColor(currentMode == vbe480pDc  ? selectedButtonColor : buttonBackColor),
        "640x480", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _settingsButton480p }, 0)) return 1;
    if (addButton((button_t){0.1f, 0.45f, 0.3f, 0.1f, getThemeColor(currentMode == vbe600pDc  ? selectedButtonColor : buttonBackColor),
        "800x600", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _settingsButton600p }, 1)) return 1;
    if (addButton((button_t){0.1f, 0.60f, 0.3f, 0.1f, getThemeColor(currentMode == vbe864pDc  ? selectedButtonColor : buttonBackColor),
        "1152x864", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _settingsButton864p }, 2)) return 1;
    if (addButton((button_t){0.1f, 0.75f, 0.3f, 0.1f, getThemeColor(currentMode == vbe1024pDc ? selectedButtonColor : buttonBackColor),
        "1280x1024", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _settingsButton1024p }, 3)) return 1;
    if (drawTextColor(0.75f, 0.15f, -1.0f, "Theme", getThemeColor(textColor), getFontSize(medium))) return 1;
    if (addButton((button_t){0.6f, 0.45f, 0.3f, 0.1f, getThemeColor(currentTheme == darkTheme  ? selectedButtonColor : buttonBackColor),
        "Dark", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _settingsButtonDarkTheme }, 4)) return 1;
    if (addButton((button_t){0.6f, 0.60f, 0.3f, 0.1f, getThemeColor(currentTheme == lightTheme ? selectedButtonColor : buttonBackColor),
        "Light", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _settingsButtonLightTheme }, 5)) return 1;
    printf("Settings Screen Drawn.\n");
    return calcStaticUI();
}

int drawScreen() {
    if (loadStaticUI()) return 1;
    //if (drawSelectedButton()) return 1;

    // TODO: Non-static components here...
    switch (currAppState) {
    case trainingState: {
        char text[10] = {0};
        sprintf(text, "%d/%d", typingCursor, generatedTextSize);
        if (drawTextWithCursor(0.5f, 0.4f, 0.8f, typingCursor, typedText, getThemeColor(textColor), getFontSize(medium))) return 1;
        if (drawTextColor(0.5f, 0.2f, -1.0f, text, getThemeColor(textColor), getFontSize(medium))) return 1;
    }   break;
    default: break;
    }
    
    if (drawCursor()) return 1;
    flip_frame(); // Update Screen
    return 0;
}

int loadScreen(){
    printf("Loading Screen...\n");
    switch (currAppState){
    case startState:        return startScreenLoad();
    case mainState:         return mainScreenLoad();
    case trainingState:     return trainingScreenLoad();
    case raceState:         return raceScreenLoad();
    case instructionsState: return instructionScreenLoad();
    case settingsState:     return settingsScreenLoad();
    case endState:          return 1;
    }
}

// LOGIC FUNCTIONS //

void generateText(uint32_t wordAmmount) {
    srand(time(NULL));
    typingCursor = 0;
    generatedText = realloc(generatedText, 1);
    generatedText[0] = 0;
    generatedTextSize = (wordAmmount == 0) ? 1 : 0;
    for (uint32_t i = 0; i < wordAmmount; i++) {
        float r = ((float) rand()) / RAND_MAX;
        size_t ind = (size_t)(r * dictionarySize);
        char* word = dictionary[ind];
        generatedTextSize += strlen(word) + 1;
        generatedText = realloc(generatedText, generatedTextSize);
        if (i) strcat(generatedText, " ");
        strcat(generatedText, word);
    }
    typedText = realloc(typedText, generatedTextSize);
    memset(typedText, 0, generatedTextSize);
    generatedTextSize--;
}
