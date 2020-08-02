//
//  SQTileMap.h
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef SQTileMap_h
#define SQTileMap_h

#include <SDL2/SDL.h>
#include <stdio.h>
#include "SQTile.h"

struct SQTileMap {
    int width;
    int height;
    
    struct SQTile *tiles;
};

struct SQTileMap SQTileMapInit(int, int, struct SQTile*);

#endif /* SQTileMap_h */
