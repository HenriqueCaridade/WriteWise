// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "typing/typing.h"
#include "lib/lib.h"
#include "ui/ui.h"

int main(int argc, char *argv[]) {
    // sets the language of LCF messages (can be either EN-US or PT-PT)
    lcf_set_language("EN-US"); 

    // enables to log function invocations that are being "wrapped" by LCF
    // [comment this out if you don't want/need/ it]
    // lcf_trace_calls("/home/lcom/labs/proj/src/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    if (argc > 2) {
        char name[50] = {0};
        sprintf(name, "/home/lcom/labs/proj/src/output%s.txt", argv[1]);
        lcf_log_output(name);
    } else {
        lcf_log_output("/home/lcom/labs/proj/src/output.txt");
    }

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
    backColor = 0,
    textColor,
    subtleColor,
    buttonBackColor,
    buttonTextColor,
    selectedButtonColor,
    errorColor,
    trainingColor,
    raceColor,
    infoColor,
} theme_color_index_t;

uint32_t darkThemeColorArray[] = {
    0x000000,
    0xFFFFFF,
    0x505050,
    0x303030,
    0xDDDDDD,
    0x606060,
    0xFF4040,
    0xFFDB58,
    0x9B4FF0,
    0x4050FF,
};

uint32_t lightThemeColorArray[] = {
    0xFFFFFF,
    0x000000,
    0xA0A0A0,
    0xC0C0C0,
    0x222222,
    0x909090,
    0xAA3030,
    0xCCA854,
    0x8B4FC0,
    0x4040CC,
};

uint32_t getThemeColor(theme_color_index_t color) {
    if (currentTheme == darkTheme) return darkThemeColorArray[color];
    else return lightThemeColorArray[color];
}

int drawScreen();
int loadScreen();
int changeState();
void keyboardScancodeHandler();
int exitAll(int code) {
    resetTypingInfo();
    freeDictionary();
    exitGraphMode(code);
    exitAllDrivers();
    return code;
}

char receivedInfo[20] = {0};
uint64_t receivedULong;
uint32_t receivedUInt;
int64_t receivedLong;
int32_t receivedInt;
double receivedDouble;
float receivedFloat;

int(proj_main_loop)(int argc, char *argv[]){
    if (initAllDrivers()) return 1;
    if (setFrameRate(60)) { exitAllDrivers(); return 1; }
    if (setMinixMode(vbe600pDc)) { exitAllDrivers(); return 1; }
    if (initUI() || loadScreen() || setTheme(darkTheme)) {
        printf("Failed Initializing UI.\n");
        return exitAll(1);
    }
    if (loadDictionary("/home/lcom/labs/proj/src/data/english.dict")) {
        printf("Failed Initializing Dictionary.\n");
        return exitAll(1);
    }
    if (setKeyboardLayout(portuguese)) {
        printf("Failed Setting Keyboard Layout.\n");
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
                if (timer_get_elapsed_count() % (frameRate >> 1) == 0) {
                    if (currAppState == trainingState) {
                        updateTypingInfo();
                    } else if (currAppState == raceState) {

                    }
                }
                if (waitingAckSerialPort && timer_get_elapsed_count() - waitingAckStartTime > ACK_TIMEOUT * frameRate) {
                    serialPortClearFIFO();
                }
                // Draw screen
                if (drawScreen(currAppState)) return exitAll(1);
            }
            if (msg.m_notify.interrupts & irqSetMouse) {
                // Mouse Interrupt
                if (mouse_int_h()) {
                    // Complete Packet
                    if (currAppState == startState && (mousePacket.lb || mousePacket.rb)) 
                        changeState(mainState);
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
            if (msg.m_notify.interrupts & irqSetSerialPort) {
                if (currAppState != raceState) break;
                // Serial Port Interrupt
                switch (receivingSerialPort) {
                    case INFO_ULONG: {
                        if (serialPortReadULong(&receivedULong) == 0) {
                            sprintf(receivedInfo, "%016llx", receivedULong);
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_LONG: {
                        if (serialPortReadLong(&receivedLong) == 0) {
                            sprintf(receivedInfo, "%016llx", receivedLong);
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_UINT: {
                        if (serialPortReadUInt(&receivedUInt) == 0) {
                            sprintf(receivedInfo, "%08x", receivedUInt);
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_INT: {
                        if (serialPortReadInt(&receivedInt) == 0) {
                            sprintf(receivedInfo, "%08x", receivedInt);
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_FLOAT: {
                        if (serialPortReadFloat(&receivedFloat) == 0) {
                            sprintf(receivedInfo, "%.4f", receivedFloat);
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_DOUBLE: {
                        if (serialPortReadDouble(&receivedDouble) == 0) {
                            sprintf(receivedInfo, "%.6lf", receivedDouble);
                            serialPortSendAck();
                        }
                    } break;
                    default: {
                        printf("Serial Interrupt.\n");
                        if (serial_port_ih()) {
                            // New Data
                            if (serialPortByte == INFO_READY) {
                                racingInfo.readyYou = true;
                                serialPortSendReadyAck();
                                break;
                            }
                            if (serialPortByte == INFO_NREADY) {
                                racingInfo.readyYou = false;
                                serialPortSendNReadyAck();
                                break;
                            }
                            if (serialPortByte == INFO_READY_ACK) {
                                racingInfo.ready = true;
                                serialPortSendReadyAck();
                                break;
                            }
                            if (serialPortByte == INFO_NREADY_ACK) {
                                racingInfo.readyYou = false;
                                serialPortSendNReadyAck();
                                break;
                            }
                            if (waitingAckSerialPort) {
                                switch (serialPortByte) {
                                case INFO_ACK: break;
                                case INFO_READY_ACK: racingInfo.readyMe = true; break;
                                case INFO_NREADY_ACK: racingInfo.readyMe = false; break;
                                default: serialPortClearFIFO(); break;
                                }
                                waitingAckSerialPort = false;
                            }
                        }
                    } break;
                }
            }
            break;
        }
        }
    }
    return exitAll(0);
}
int changeState(app_state_t newState) {
    printf("Changed From %d to %d.\n", currAppState, newState);
    currAppState = newState;
    switch (newState) {
    case endState: return 0;
    case trainingState: generateText(25, time(NULL)); break;
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
            typingInputHandler();
        }
        if (isScancodeTwoBytes) {
            switch (scancode) {
                default: return;
            }
        } else {
            switch (scancode) {
                case KEY_SCANCODE_B: serialPortSendByte((uint8_t) timer_get_elapsed_count()); break;
                case KEY_SCANCODE_I:
                    printf("Sending Int\n");
                    if (isShiftPressed) {
                        serialPortSendUInt((uint32_t) timer_get_elapsed_count());
                    } else {
                        serialPortSendInt((int32_t) timer_get_elapsed_count());
                    } break;
                case KEY_SCANCODE_L:
                    printf("Sending Long\n");
                    if (isShiftPressed) {
                        serialPortSendULong(timer_get_elapsed_count());
                    } else {
                        serialPortSendLong((int64_t) timer_get_elapsed_count());
                    } break;
                case KEY_SCANCODE_F:
                    printf("Sending Float\n");
                    if (isShiftPressed) {
                        serialPortSendDouble((double) timer_get_elapsed_count() / 20.0);
                    } else {
                        serialPortSendFloat ((float) timer_get_elapsed_count() / 20.0f);
                    } break;
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
    if (drawTextColor(0.5f, 0.15f, -1.0f, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to exit...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (addButton((button_t){0.3f, 0.30f, 0.4f, 0.1f, getThemeColor(trainingColor)  , "Training"    , getThemeColor(backColor), getFontSize(medium), getThemeColor(textColor), _mainButtonTraining    }, 0)) return 1;
    if (addButton((button_t){0.3f, 0.45f, 0.4f, 0.1f, getThemeColor(raceColor)      , "Race"        , getThemeColor(backColor), getFontSize(medium), getThemeColor(textColor), _mainButtonRace        }, 1)) return 1;
    if (addButton((button_t){0.3f, 0.60f, 0.4f, 0.1f, getThemeColor(infoColor)      , "Instructions", getThemeColor(backColor), getFontSize(medium), getThemeColor(textColor), _mainButtonInstructions}, 2)) return 1;
    if (addButton((button_t){0.3f, 0.75f, 0.4f, 0.1f, getThemeColor(buttonBackColor), "Settings"    , getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _mainButtonSettings    }, 3)) return 1;
    printf("Main Screen Drawn.\n");
    return calcStaticUI();
}

void _trainingButtonReset(void) { generateText(25, time(NULL)); }
int trainingScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Training Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Training", getThemeColor(trainingColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (addButton((button_t){0.4f, 0.70f, 0.2f, 0.1f, getThemeColor(buttonBackColor), "New Test", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _trainingButtonReset}, 0)) return 1;
    printf("Training Screen Drawn.\n"); 
    return calcStaticUI();
}


void _raceReadyButton(void) {
    if (!racingInfo.readyMe) {
        serialPortSendReady();
    }
    generateText(25, time(NULL));
}
int raceScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Race Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Race", getThemeColor(raceColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (addButton((button_t){0.4f, 0.70f, 0.2f, 0.1f, getThemeColor(buttonBackColor), "Ready", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _trainingButtonReset}, 0)) return 1;
    printf("Race Screen Drawn.\n");
    return calcStaticUI();
}
int instructionScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Instructions Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Instructions", getThemeColor(infoColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (drawTextColor(0.5f, 0.2f, -1.0f, "Training", getThemeColor(trainingColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.3f,  0.8f,
        "In the Training section you can train your typing speed before you race others. It provides a safe environment where you can practice without worring about others.",
        getThemeColor(textColor), getFontSize(large))) return 1;
    if (drawTextColor(0.5f, 0.5f, -1.0f, "Race", getThemeColor(raceColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.6f,  0.8f,
        "In the Race section you will race an opponent and the first to finish typing the given words will win!",
        getThemeColor(textColor), getFontSize(large))) return 1;
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
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Settings", getThemeColor(subtleColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
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
    // if (drawSelectedButton()) return 1;
    
    switch (currAppState) {
    case trainingState: {
        char infoText[50] = {0};
        sprintf(infoText, "%.2lf%% %.2lfwpm %d/%d", typingInfo.acc, typingInfo.wpm, typingInfo.typedChars, typingInfo.generatedTextSize);
        if (drawTypingTest(0.5f, 0.3f, 0.8f, getThemeColor(textColor), getThemeColor(errorColor), getThemeColor(subtleColor), getFontSize(medium))) return 1;
        if (drawTextColor(0.5f, 0.2f, -1.0f, infoText, getThemeColor(trainingColor), getFontSize(large))) return 1;
    } break;
    case raceState: {
        if (drawTextColor(0.5f, 0.5f, -1.0f, receivedInfo, getThemeColor(textColor), getFontSize(large))) return 1;
    } break;
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
