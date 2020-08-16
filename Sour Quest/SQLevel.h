//
//  SQLevel.h
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef SQLevel_h
#define SQLevel_h

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <Chipmunk/chipmunk.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "Utils.h"
#include "cJSON/cJSON.h"
#include "SQTileMap.h"

struct SQLevel {
    SDL_Renderer *renderer;
    struct SQTileMap tilemap;
    cpVect cameraPosition;
    
};

struct SQLevel SQLevelInit(SDL_Renderer*, struct SQTileMap, cpVect);

int SQLevel_ReadFromFile(struct SQLevel*, struct SQTileMap*, struct SQArray*, SDL_Renderer*, const char* fileName);

int SQLevel_SetBackgroundColor(struct SQLevel*, SDL_Color);

void SQLevel_Render(struct SQLevel, SDL_Texture*);

#endif /* SQLevel_h */
