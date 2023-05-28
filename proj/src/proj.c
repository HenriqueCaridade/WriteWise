// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>
#include <math.h>

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

/**
 * @brief Retrieves the color value for a specific theme color index.
 *
 * This function returns the color value associated with the specified theme color index. The color value is determined based on the currently selected theme (dark or light).
 *
 * @param color The theme color index.
 * @return The color value corresponding to the theme color index.
 */
uint32_t getThemeColor(theme_color_index_t color) {
    if (currentTheme == darkTheme) return darkThemeColorArray[color];
    else return lightThemeColorArray[color];
}
/**
 * @brief Draws the user interface on the screen.
 *
 * This function is responsible for drawing the UI on the screen. It handles drawing various UI elements such as static UI components, real-time information, selected buttons, typing test, countdown and the cursor. The appearance and layout of the UI elements depend on the current theme and state.
 *
 * @return 0 on success, 1 on failure.
 */
int drawScreen();
/**
 * @brief Loads the appropriate screen based on the current application state.
 *
 * This function determines the screen to load based on the value of `currAppState` and calls the corresponding screen loading function for each application state.
 *
 * @return 0 if the screen was loaded successfully, 1 otherwise.
 */
int loadScreen();

/**
 * @brief Changes the current application state to the specified new state.
 *
 * @param newState The new application state to transition to.
 * @return 0 if successful, 1 otherwise.
 */
int changeState(app_state_t newState);

/**
 * @brief Handles keyboard scancodes and performs corresponding actions.
 *
 * This function is responsible for handling keyboard scancodes and taking appropriate actions based on the scancode received. It distinguishes between make codes and break codes and performs specific actions accordingly.
 *
 * @return void
 */
void keyboardScancodeHandler();
/**
 * @brief Exits the program and performs cleanup operations.
 *
 * It resets the typing information, frees the allocated dictionary memory, exits the graphics mode,
 * and shuts down all drivers before returning the exit code.
 *
 * @param code The exit code to be returned.
 * @return The exit code provided as input.
 */
int exitAll(int code) {
    resetTypingInfo();
    freeDictionary();
    exitGraphMode(code);
    exitAllDrivers();
    return code;
}

uint64_t receivedULong;
uint32_t receivedUInt;
int64_t receivedLong;
int32_t receivedInt;
double receivedDouble;
float receivedFloat;

char readyButtonText[8] = {0};
char popupText[5] = {0};

bool waitingForSeed = false;

/**
 * @brief Checks if both players are ready and generates text for the race.
 *
 * This function checks if both the local player and the opponent are ready, and if their respective
 * seed values are non-zero. If all conditions are met, it calculates the seed value, generates text
 * for the race using the calculated seed, and sets the start time for the race.
 */
void checkIfReady() {
    if (racingInfo.readyMe && racingInfo.readyYou && racingInfo.seedMe != 0 && racingInfo.seedYou != 0) {
        uint64_t seed = racingInfo.seedMe + racingInfo.seedYou;
        generateText(25, seed);
        racingInfo.startTime = 5 * frameRate + timer_get_elapsed_count();
    }
}
/**
 * @brief Main loop of the project.
 *
 * This function initializes all necessary drivers, sets the frame rate, and initializes the user interface.
 * It also loads the initial screen and theme, as well as the dictionary for text generation.
 * The function has a loop where it waits for various interrupts and handles them accordingly.
 * The function continues to run until the current application state is set to the end state, at which point it
 * returns the exit code.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return The exit code of the program.
 */
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
                if (timer_get_elapsed_count() % frameRate == 0) {
                    // Update every second.
                    rtcUpdate();
                }
                if (currAppState == trainingState) {
                    if (timer_get_elapsed_count() % (frameRate >> 1) == 0) {
                        updateTypingInfo();
                    } 
                } else if (currAppState == raceState) {
                    if (timer_get_elapsed_count() % (frameRate >> 1) == 0) {
                        updateTypingInfo();
                    } 
                    if (typingInfo.status == ready) {
                        int64_t timeLeftInt = racingInfo.startTime - timer_get_elapsed_count();
                        int timeLeft = (int)(floor((double)(timeLeftInt) / frameRate)) + 1;
                        if (timeLeft <= 0) {
                            startTyping();
                        } else sprintf(popupText, "%d", timeLeft);
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
                            // Unsigned Long Ready
                            if (waitingForSeed) {
                                racingInfo.seedYou = receivedULong;
                                printf("Seed Received: %016llx\n", racingInfo.seedYou);
                                checkIfReady();
                                waitingForSeed = false;
                            }
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_LONG: {
                        if (serialPortReadLong(&receivedLong) == 0) {
                            // Long Ready
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_UINT: {
                        if (serialPortReadUInt(&receivedUInt) == 0) {
                            // Unsigned Int Ready
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_INT: {
                        if (serialPortReadInt(&receivedInt) == 0) {
                            // Int Ready
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_FLOAT: {
                        if (serialPortReadFloat(&receivedFloat) == 0) {
                            // Float Ready
                            serialPortSendAck();
                        }
                    } break;
                    case INFO_DOUBLE: {
                        if (serialPortReadDouble(&receivedDouble) == 0) {
                            // Double Ready
                            serialPortSendAck();
                        }
                    } break;
                    default: {
                        if (serial_port_ih()) {
                            printf("%02x\n", serialPortByte);
                            if (typingInfo.status == unready){
                                if (serialPortByte == INFO_READY) {
                                    racingInfo.readyYou = true;
                                    waitingForSeed = true;
                                    serialPortSendReadyAck();
                                    break;
                                }
                                if (serialPortByte == INFO_NREADY) {
                                    racingInfo.readyYou = false;
                                    serialPortSendNReadyAck();
                                    break;
                                }
                            } else if (typingInfo.status == typing) {
                                if (serialPortByte == INFO_WON) {
                                    // Opponent Won
                                    endTyping();
                                    racingInfo.won = false;
                                }
                            }
                            if (serialPortByte == INFO_EXIT) {
                                // Opponent Left
                                resetTypingInfo();
                                resetRacingInfo();
                            }
                            if (waitingAckSerialPort) {
                                switch (serialPortByte) {
                                case INFO_ACK: break;
                                case INFO_READY_ACK: 
                                    racingInfo.readyMe = true;
                                    checkIfReady();
                                    break;
                                case INFO_NREADY_ACK:
                                    racingInfo.readyMe = false;
                                    racingInfo.seedMe = 0;
                                    break;
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
/**
 * Changes the current state of the application.
 *
 * This function is responsible for changing the state of the application based on the provided new state.
 *
 * @param newState new state to transition to.
 * @return 0 on success, 1 on failure.
 */
int changeState(app_state_t newState) {
    printf("Changed From %d to %d.\n", currAppState, newState);
    if (currAppState == raceState) serialPortSendExit();
    currAppState = newState;
    switch (newState) {
    case endState: return 0;
    case trainingState: generateText(25, time(NULL)); break;
    case raceState: resetTypingInfo(); resetRacingInfo(); serialPortClearFIFO(); break;
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
        if (currAppState == trainingState || currAppState == raceState) {
            typingInputHandler(currAppState == raceState);
        }
    }
}
/**
 * @brief Loads the start screen.
 *
 * This function is responsible for loading and displaying the start screen of the application.
 *
 * @return 0 on success, 1 on failure.
 */
int startScreenLoad() {
    // TEST UI
    clearScreen();
    clearButtons();
    if (drawTextColor(0.5f, 0.30f, -1.0f, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press any key to start...", getThemeColor(subtleColor), getFontSize(medium))) return 1;
    return calcStaticUI();
}
/**
 * @brief function for the Training button in the main screen.
 *
 * This function is called when the Training button is clicked. It changes the state to the training state, which typically leads to the display of the training screen.
 */
void _mainButtonTraining    (void) { changeState(trainingState);     }
/**
 * @brief Callback function for the Race button in the main screen.
 *
 * This function is called when the Race button is clicked. It changes the state to the race state, which typically leads to the display of the race screen.
 */
void _mainButtonRace        (void) { changeState(raceState);         }
/**
 * @brief Callback function for the Instructions button in the main screen.
 *
 * This function is called when the Instructions button is clicked. It changes the state to the instructions state, which typically leads to the display of the instructions screen.
 */
void _mainButtonInstructions(void) { changeState(instructionsState); }
/**
 * @brief Callback function for the Settings button in the main screen.
 *
 * This function is called when the Settings button is clicked. It changes the state to the settings state, which typically leads to the display of the settings screen.
 */
void _mainButtonSettings    (void) { changeState(settingsState);     }
/**
 * @brief Loads the main screen.
 *
 * This function clears the screen and buttons, and then proceeds to draw various elements on the screen such as text and buttons.
 * @return 0 on success, 1 on failure.
 */
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
    if (drawButtons()) return 1;
    printf("Main Screen Drawn.\n");
    return calcStaticUI();
}

void _trainingButtonReset(void) { generateText(25, time(NULL)); }
/**
 * @brief Loads the training screen.
 *
 * This function clears the screen and buttons, and then proceeds to draw various elements on the screen such as text and buttons.
 *
 * @return 0 on success, 1 on failure.
 */
int trainingScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Training Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Training", getThemeColor(trainingColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (addButton((button_t){0.4f, 0.70f, 0.2f, 0.1f, getThemeColor(buttonBackColor), "New Test", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _trainingButtonReset}, 0)) return 1;
    if (drawButtons()) return 1;
    printf("Training Screen Drawn.\n"); 
    return calcStaticUI();
}
/**
 * @brief Race ready button handler.
 * 
 * This function is called when the "Ready" button is clicked.
 * It checks the typing status and performs the corresponding actions.
 * If the typing status is "unready", it either sends a "Not Ready" signal or sends the "Ready" signal along with the seed.
 */
void _raceReadyButton(void) {
    if (typingInfo.status == unready) {
        if (racingInfo.readyMe) serialPortSendNReady();
        else {
            racingInfo.seedMe = time(NULL);
            printf("My Seed: %016llx\n", racingInfo.seedMe);
            serialPortSendReady();
            serialPortSendULong(racingInfo.seedMe);
        }
    }
}
/**
 * @brief Race reset button handler.
 * 
 * This function is called when the "New Race" button is clicked.
 */
void _raceResetButton(void) {
    if (typingInfo.status == finished) {
        resetTypingInfo();
        resetRacingInfo();
    }
}
/**
 * @brief Loads the race screen.
 * 
 * This function clears the screen, clears the buttons, and then proceeds to draw the race screen UI elements.
 * It displays the "Write Wise" and "Race" text, along with the ready button.
 * It also adds the "Ready" and "New Race" buttons.
 * Finally, it calculates the static UI elements.
 * 
 * @return 0 on success, 1 on failure.
 */
int raceScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Race Started Drawing.\n");
    strcpy(readyButtonText, "Ready");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Race", getThemeColor(raceColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (addButton((button_t){0.4f, 0.70f, 0.2f, 0.1f, getThemeColor(buttonBackColor), readyButtonText, getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _raceReadyButton}, 0)) return 1;
    if (addButton((button_t){0.4f, 0.70f, 0.2f, 0.1f, getThemeColor(buttonBackColor), "New Race", getThemeColor(buttonTextColor), getFontSize(medium), getThemeColor(textColor), _raceResetButton}, 1)) return 1;
    printf("Race Screen Drawn.\n");
    return calcStaticUI();
}
/**
 * @brief Loads the instructions screen.
 * 
 * This function clears the screen, clears the buttons, and then proceeds to draw the instructions screen UI elements.
 * @return 0 on success, 1 on failure.
 */
int instructionScreenLoad() {
    clearScreen();
    clearButtons();
    printf("Instructions Started Drawing.\n");
    if (drawTextXYColor(10, 10, -1, "Write Wise", getThemeColor(textColor), getFontSize(xxxlarge))) return 1;
    if (drawTextXYColor(10, 15 + getFontSize(xxxlarge).height, -1, "Instructions", getThemeColor(infoColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, -1.0f, "Press ESC to go back to the menu...", getThemeColor(subtleColor), getFontSize(small))) return 1;
    if (drawTextColor(0.5f, 0.2f, -1.0f, "Training", getThemeColor(trainingColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.3f,  0.8f,
        "In the Training section you can train your typing speed before you race others. It provides a safe environment where you can practice without worrying about others.",
        getThemeColor(textColor), getFontSize(large))) return 1;
    if (drawTextColor(0.5f, 0.5f, -1.0f, "Race", getThemeColor(raceColor), getFontSize(xlarge))) return 1;
    if (drawTextColor(0.5f, 0.6f,  0.8f,
        "In the Race section you will race an opponent and the first to finish typing the given words will win!",
        getThemeColor(textColor), getFontSize(large))) return 1;
    if (drawButtons()) return 1;
    printf("Instructions Screen Drawn.\n");
    return calcStaticUI();
}
/**
 * @brief Loads the settings screen.
 * 
 * This function clears the screen, clears the buttons, and then proceeds to draw the settings screen UI elements.
 * @return 0 on success, 1 on failure.
 */
int settingsScreenLoad();
/**
 * @brief Sets the screen resolution to 480p, initializes the UI, and loads the settings screen.
 * 
 * This function sets the screen resolution to 480p, initializes the UI, and then loads the settings screen.
 */
void _settingsButton480p      (void) { exitUI(); setMinixMode(vbe480pDc);  initUI(); settingsScreenLoad(); }
/**
 * @brief Sets the screen resolution to 600p, initializes the UI, and loads the settings screen.
 * 
 * This function sets the screen resolution to 600p, initializes the UI, and then loads the settings screen.
 */
void _settingsButton600p      (void) { exitUI(); setMinixMode(vbe600pDc);  initUI(); settingsScreenLoad(); }
/**
 * @brief Sets the screen resolution to 864p, initializes the UI, and loads the settings screen.
 * 
 * This function sets the screen resolution to 864p, initializes the UI, and then loads the settings screen.
 */
void _settingsButton864p      (void) { exitUI(); setMinixMode(vbe864pDc);  initUI(); settingsScreenLoad(); }
/**
 * @brief Sets the screen resolution to 1024p, initializes the UI, and loads the settings screen.
 * 
 * This function sets the screen resolution to 1024p, initializes the UI, and then loads the settings screen.
 */
void _settingsButton1024p     (void) { exitUI(); setMinixMode(vbe1024pDc); initUI(); settingsScreenLoad(); }
/**
 * @brief Sets the dark theme and loads the settings screen.
 * 
 * This function sets the theme to the dark theme and then loads the settings screen.
 */
void _settingsButtonDarkTheme (void) { setTheme(darkTheme);  settingsScreenLoad(); }
/**
 * @brief Sets the light theme and loads the settings screen.
 * 
 * This function sets the theme to the light theme and then loads the settings screen.
 */
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
    if (drawButtons()) return 1;
    printf("Settings Screen Drawn.\n");
    return calcStaticUI();
}

int drawScreen() {
    if (loadStaticUI()) return 1;
    if (currAppState != startState) if (drawRealTime(getThemeColor(subtleColor), getFontSize(medium))) return 1;
    if (drawSelectedButton()) return 1;
    
    switch (currAppState) {
    case trainingState: {
        if (typingInfo.status == typing || typingInfo.status == finished) {
            char   accText[10] = {0}; sprintf(accText, "%.2lf%%", typingInfo.acc);
            char   wpmText[10] = {0}; sprintf(wpmText,   "%.2lf", typingInfo.wpm);
            char charsText[10] = {0}; sprintf(charsText, "%d/%d", typingInfo.typedChars, typingInfo.generatedTextSize);
            if (drawTextColor(0.3f, 0.25f, -1.0f,   accText, getThemeColor(trainingColor), getFontSize(large))) return 1;
            if (drawTextColor(0.5f, 0.25f, -1.0f,   wpmText, getThemeColor(trainingColor), getFontSize(large))) return 1;
            if (drawTextColor(0.7f, 0.25f, -1.0f, charsText, getThemeColor(trainingColor), getFontSize(large))) return 1;
            if (drawTextColor(0.3f, 0.31f, -1.0f, "accuracy", getThemeColor(subtleColor), getFontSize(medium))) return 1;
            if (drawTextColor(0.5f, 0.31f, -1.0f,      "wpm", getThemeColor(subtleColor), getFontSize(medium))) return 1;
            if (drawTextColor(0.7f, 0.31f, -1.0f, "progress", getThemeColor(subtleColor), getFontSize(medium))) return 1;
        }
        if (typingInfo.status != unready) if (drawTypingTest(0.5f, 0.4f, 0.8f, getThemeColor(textColor), getThemeColor(errorColor), getThemeColor(subtleColor), getFontSize(medium))) return 1;
    } break;
    case raceState: {
        switch (typingInfo.status) {
            case unready:
                strcpy(readyButtonText, racingInfo.readyMe ? "Unready" : "Ready");
                if (drawButton(0)) return 1;
                char* readyYouText = racingInfo.readyYou ? "Other player is ready." : "Other player isn't ready.";
                uint32_t readyYouColor = racingInfo.readyYou ? getThemeColor(textColor) : getThemeColor(errorColor);
                if (drawTextColor(0.5f, 0.5f, -1.0f, readyYouText, readyYouColor, getFontSize(large))) return 1;
                break;
            default:
                if (drawTypingTest(0.5f, 0.4f, 0.8f, getThemeColor(textColor), getThemeColor(errorColor), getThemeColor(subtleColor), getFontSize(medium))) return 1;
                break;
        }
        if (typingInfo.status == typing || typingInfo.status == finished) {
            char   accText[10] = {0}; sprintf(accText, "%.2lf%%", typingInfo.acc);
            char   wpmText[10] = {0}; sprintf(wpmText,   "%.2lf", typingInfo.wpm);
            char charsText[10] = {0}; sprintf(charsText, "%d/%d", typingInfo.typedChars, typingInfo.generatedTextSize);
            if (drawTextColor(0.3f, 0.25f, -1.0f,   accText, getThemeColor(raceColor), getFontSize(large))) return 1;
            if (drawTextColor(0.5f, 0.25f, -1.0f,   wpmText, getThemeColor(raceColor), getFontSize(large))) return 1;
            if (drawTextColor(0.7f, 0.25f, -1.0f, charsText, getThemeColor(raceColor), getFontSize(large))) return 1;
            if (drawTextColor(0.3f, 0.31f, -1.0f, "accuracy", getThemeColor(subtleColor), getFontSize(medium))) return 1;
            if (drawTextColor(0.5f, 0.31f, -1.0f,      "wpm", getThemeColor(subtleColor), getFontSize(medium))) return 1;
            if (drawTextColor(0.7f, 0.31f, -1.0f, "progress", getThemeColor(subtleColor), getFontSize(medium))) return 1;
        }
        if (typingInfo.status == ready) // Countdown
            if (drawTextColor (0.5f, 0.5f, -1.0f, popupText, getThemeColor(raceColor), getFontSize(xxxlarge))) return 1;
        if (typingInfo.status == finished) { // Won / Lost
            strcpy(popupText, racingInfo.won ? "Won" : "Lost");
            if (drawTextColor (0.5f, 0.5f, -1.0f, popupText, getThemeColor(raceColor), getFontSize(xxxlarge))) return 1;
            if (drawButton(1)) return 1;
        }
        
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
