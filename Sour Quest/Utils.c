//
//  Utils.c
//  Sour Quest
//
//  Created by Jorge on 8/1/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "Utils.h"

char* stringFromFileNamed(const char* fileName) {
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
        absolute = SDL_GetBasePath();
        absolute = absolute ? absolute : "./";
    }
    
    long absoluteLength = strlen(absolute);
    long relativeLength = strlen(relative);
    char* ret = malloc((absoluteLength + relativeLength) * sizeof(char));
    if (!ret) { return NULL; }
    strcat(ret, absolute);
    strcat(ret, relative);
    
    return ret;
}

void sqFree(void* pointer) {
    free(pointer);
    pointer = NULL;
}
