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
    lcf_trace_calls("/home/lcom/labs/lab4/trace.txt");

    // enables to save the output of printf function calls on a file
    // [comment this out if you don't want/need it]
    lcf_log_output("/home/lcom/labs/lab4/output.txt");

    // handles control over to LCF
    // [LCF handles command line arguments and invokes the right function]
    if (lcf_start(argc, argv))
        return 1;

    // LCF clean up tasks
    // [must be the last statement before return]
    lcf_cleanup();

    return 0;
}

int drawScreen();
int loadUI();

int(proj_main_loop)(int argc, char *argv[]){
    if (initAllDrivers()) return 1;
    if (setFrameRate(60)) return 1;
    if (setMinixMode(vbe600pDc)) return 1;
    if (initUI() || loadUI()) { exit_graphic_mode(); return 1; }
    bool running = true;
    while (running) {
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
                if (drawScreen()) { exit_graphic_mode(); return 1; }
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
                    if (isScancodeTwoBytes) { // Scancode available is Two Bytes
                        // Do something...
                    } else { // Normal Scancode
                        if (scancode == BREAK_ESC) {
                            running = false;
                            break;
                        }
                    }
                }
            }
            break;
        }
        }
    }
    exit_graphic_mode();
    return exitAllDrivers();
}

uint32_t currColorPhase = 0;
uint32_t testColors[4] = {0xFFFFFF, 0x40FF40, 0xFF00FF, 0x4040FF};


void onClickTestButton(void) {
    currColorPhase++;
    loadUI();
}

int testUI(){
    // TEST UI
    clearScreen();
    clearButtons();
    if (drawTextColor(20, 20, "THE QUICK BROWN FOX JUMPS OF THE LAZY DOG", testColors[currColorPhase % 4])) return 1;
    if (drawTextColor(20, 40, "the quick brown fox jumps of the lazy dog", testColors[(currColorPhase + 1) % 4])) return 1;
    if (drawTextColor(20, 60, "0123456789", testColors[(currColorPhase + 2) % 4])) return 1;
    if (drawTextColor(20, 80, "a.a,a;a:", testColors[(currColorPhase + 3) % 4])) return 1;
    if (addButton((button_t){20, 100, 100, 40, 0xFFFFFF, 0, 0xFF0000, "Test", 0x202020, onClickTestButton}, 0)) return 1;
    return 0;
}

int drawScreen(){
    if (loadStaticUI()) return 1;
    if (drawCursor()) return 1;
    flip_frame(); // Update Screen
    return 0;
}

int loadUI(){
    if (testUI()) return 1;
    return calcStaticUI();
}
