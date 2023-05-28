#ifndef _PROJ_DATA_H
#define _PROJ_DATA_H

#define MAX_WORD_AMMOUNT 1000

#include <stdint.h>
#include <stdbool.h>
#include "../lib/lib.h"

size_t dictionarySize;
char * dictionary[MAX_WORD_AMMOUNT];

/**
 * @brief Creates a new dictionary from a file.
 * 
 * @param filename 
 * @return Non-zero if error occured.
 */
int loadDictionary(const char* filename);

/**
 * @brief Frees dictionary.
 * 
 */
void freeDictionary();

#endif // _PROJ_DATA_H
