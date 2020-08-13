//
//  Functions.h
//  Sour Quest
//
//  Created by Jorge on 8/8/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef Functions_h
#define Functions_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <assert.h>
#include <SDL2/SDL.h>


uint nextPow2(uint n);

char* fileToString(const char*);

char* pathRelativeToResources(const char*);

#define sqFree(pointer) free(pointer);pointer=NULL;

#endif /* Functions_h */
