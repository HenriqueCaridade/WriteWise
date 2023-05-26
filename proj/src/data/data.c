
#include "data.h"

size_t dictionarySize = 0;
char* dictionary[MAX_WORD_AMMOUNT] = { NULL };

int loadDictionary(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        dictionarySize = 0;
        printf("File \"%s\" not found!\n", filename);
        return 1;
    }

    freeDictionary();
    char* line = NULL;
    size_t bufferLen = 0;
    ssize_t lineLen = 0;
    size_t lineNum = 0;
    
    while ((lineNum < MAX_WORD_AMMOUNT) && ((lineLen = getline(&line, &bufferLen, file)) != -1)) {
        dictionary[lineNum] = (char*) malloc(lineLen - 1);
        memcpy(dictionary[lineNum], line, lineLen - 2);
        dictionary[lineNum][lineLen - 2] = '\0';
        lineNum++;
    }
    dictionarySize = lineNum;
    while (lineNum < MAX_WORD_AMMOUNT) dictionary[lineNum++] = NULL;
    printf("Loaded Dictionary \"%s\" Successfully!\n", filename);
    close((int) file);
    return 0;
}

void freeDictionary() {
    for (int i = 0; i < MAX_WORD_AMMOUNT; i++) if (dictionary[i] != NULL) free(dictionary[i]);
}
