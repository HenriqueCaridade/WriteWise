#ifndef _PROJ_LIB_H
#define _PROJ_LIB_H

int setFrameRate(uint16_t fps);

int setTimerCallback(void (*voidCallback)(void));
int setKeyboardCallback(void (*voidCallback)(void));



int initAll();
int exitAll();

#endif // _PROJ_LIB_H
