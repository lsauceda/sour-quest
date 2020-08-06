//
//  Utils.h
//  Sour Quest
//
//  Created by Jorge on 8/1/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef Utils_h
#define Utils_h

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <SDL2/SDL.h>

char* fileToString(const char*);

char* pathRelativeToResources(const char*);

#define sqFree(pointer) free(pointer);pointer=NULL;

#define SQArray(type) struct { int lenght; type *items; }

#endif /* Utils_h */
