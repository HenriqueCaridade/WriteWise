#include "lib.h"

int setFrameRate(uint16_t fps){
    if (fps < 20 || fps > 60) {
        printf("Frame Rate must be set between 20 and 60.\n");
        return 0;
    }
    return timer_set_frequency(0, fps);
}

int setMinixMode(minix_mode_t mode){
    if (mode == textMode){
        if (set_text_mode()) return 1;
    } else {
        if (set_graphic_mode(mode)) return 1;
    }
    return 0;
}

int initAll(){
    isScancodeTwoBytes = false;
    if (timer_subscribe_interrupt()) return 1;
    if (keyboard_subscribe_int()) return 1;
    if (mouse_subscribe_int()) return 1;
    return 0;
}

int exitAll(){
    if (mouse_unsubscribe_int()) return 1;
    if (keyboard_unsubscribe_int()) return 1;
    if (timer_unsubscribe_int()) return 1;
    return 0;
}

