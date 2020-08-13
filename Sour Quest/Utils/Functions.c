//
//  Functions.c
//  Sour Quest
//
//  Created by Jorge on 8/8/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "Functions.h"

uint nextPow2(uint n) {
    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
#if WORD_BIT == 64
    n |= n >> 32;
#endif
    return n;
}

char* fileToString(const char* fileName) {
    assert(fileName != NULL);
    
    FILE *file = fopen(fileName, "r");
    if (!file) { return NULL; }
    
    if (fseek(file, 0, SEEK_END)) { return NULL; }
    long length = ftell(file);
    if (length < 0) { return NULL; }
    if (fseek(file, 0, SEEK_SET)) { return NULL; }
    
    char *text = malloc((length * sizeof(char)) + 1);
    if (!text) { return NULL; }
    
    length = fread(text, sizeof(char), length, file);
    text[length] = '\0';
    fclose(file);
    
    return text;
}

char* pathRelativeToResources(const char* relative) {
    assert(relative != NULL);
    
    static char *absolute = NULL;
    if (!absolute) {
        // FIXME: This should get deallocated at some point
        absolute = SDL_GetBasePath();
        absolute = absolute ? absolute : "./";
    }
    
    long absoluteLength = strlen(absolute);
    long relativeLength = strlen(relative);
    long stringLength = absoluteLength + relativeLength + 1;
    char* ret = malloc((absoluteLength + relativeLength + 1) * sizeof(char));
    if (!ret) { return NULL; }
    snprintf(ret, stringLength, "%s%s", absolute, relative);
    
    return ret;
}
