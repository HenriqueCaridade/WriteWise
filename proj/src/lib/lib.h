#ifndef _PROJ_LIB_H
#define _PROJ_LIB_H

// LCOM Library
#include <lcom/lcf.h>

// Driver Files
#include <lcom/timer.h>
#include "drivers/timer.h"
#include "drivers/graphics.h"
#include "drivers/keyboard.h"
#include "drivers/mouse.h"

// Macros
#include "drivers/i8254.h"
#include "drivers/i8042.h"

message msg;
int ipcStatus;

typedef enum {
    textMode = 0,
    vbe768pInd = 0x105,
    vbe480pDc = 0x110,
    vbe600pDc = 0x115,
    vbe1024pDc = 0x11A,
    vbe864pDc = 0x14C
} minix_mode_t;

int setFrameRate(uint16_t fps);

int setMinixMode(minix_mode_t mode);

int initAll();

int exitAll();

#endif // _PROJ_LIB_H
