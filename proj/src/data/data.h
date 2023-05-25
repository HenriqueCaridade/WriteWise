#ifndef _PROJ_DATA_H
#define _PROJ_DATA_H

#define MAX_WORD_AMMOUNT 1000

#include <stdint.h>
#include <stdbool.h>
#include "../lib/lib.h"

size_t dictionarySize;
char * dictionary[MAX_WORD_AMMOUNT];

int loadDictionary(const char* filename);
void freeDictionary();

#endif // _PROJ_DATA_H
