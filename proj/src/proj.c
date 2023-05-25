// IMPORTANT: you must include the following line in all your C files
#include <lcom/lcf.h>

#include <stdint.h>
#include <stdio.h>

// Any header files included below this line should have been created by you
#include "lib/lib.h"
#include "ui/ui.h"

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
    instructionState,
    endState,
} app_state_t;

app_state_t currAppState = startState;
int drawScreen();
int loadScreen();
int changeState();
void keyboardScancodeHandler();

int(proj_main_loop)(int argc, char *argv[]){
    if (initAllDrivers()) return 1;
    if (setFrameRate(60)) { exitAllDrivers(); return 1; }
    if (setMinixMode(vbe600pDc)) { exitAllDrivers(); return 1; }
    if (initUI() || loadScreen()) { vg_exit(); exitAllDrivers();  return 1; }
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
                if (drawScreen(currAppState)) { vg_exit(); exitAllDrivers(); return 1; }
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
    vg_exit();
    return exitAllDrivers();
}
int changeState(app_state_t newState){
    printf("Changed From %d to %d.\n", currAppState, newState);
    currAppState = newState;
    if (newState == endState) return 0;
    return loadScreen();
}

void keyboardScancodeHandler() {
    if (scancode & MAKE_CODE) {
        // BREAK CODE
        if (!isScancodeTwoBytes && scancode == BREAK_ESC) changeState(endState); return;
    } else {
        // MAKE CODE
        if (currAppState == startState) {
            changeState(mainState);
            return;
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
    if (drawTextColor(0.5f, 0.30f, "Write Wise", 0xFFFFFF, getFontSize(xxxlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, "Press any key to start...", 0x505050, getFontSize(medium))) return 1;
    return calcStaticUI();
}

void _mainButtonTraining(void)     { changeState(trainingState);    }
void _mainButtonRace(void)         { changeState(raceState);        }
void _mainButtonInstructions(void) { changeState(instructionState); }
int mainScreenLoad() {
    clearScreen();
    clearButtons();
    if (drawTextColor(0.5f, 0.15f, "Write Wise", 0xFFFFFF, getFontSize(xxlarge))) return 1;
    if (drawTextColor(0.5f, 0.95f, "Press ESC to exit...", 0x505050, getFontSize(medium))) return 1;
    if (addButton((button_t){0.3f, 0.30f, 0.4f, 0.1f, 0xAFAFAF, "Training"    , 0x202020, getFontSize(medium), _mainButtonTraining    }, 0)) return 1;
    if (addButton((button_t){0.3f, 0.45f, 0.4f, 0.1f, 0xAFAFAF, "Race"        , 0x202020, getFontSize(medium), _mainButtonRace        }, 1)) return 1;
    if (addButton((button_t){0.3f, 0.60f, 0.4f, 0.1f, 0xAFAFAF, "Instructions", 0x202020, getFontSize(medium), _mainButtonInstructions}, 2)) return 1;
    return calcStaticUI();
}

int trainingScreenLoad() {
    clearScreen();
    clearButtons();
    return calcStaticUI();
}
int raceScreenLoad() {
    clearScreen();
    clearButtons();
    return calcStaticUI();
}
int instructionScreenLoad() {
    clearScreen();
    clearButtons();
    return calcStaticUI();
}

int drawScreen() {
    if (loadStaticUI()) return 1;
    if (drawCursor()) return 1;
    flip_frame(); // Update Screen
    return 0;
}

int loadScreen(){
    switch (currAppState){
    case startState:       return startScreenLoad();
    case mainState:        return mainScreenLoad();
    case trainingState:    return trainingScreenLoad();
    case raceState:        return raceScreenLoad();
    case instructionState: return instructionScreenLoad();
    case endState:         return 1;
    }
}
