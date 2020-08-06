//
//  SQTile.h
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef SQTile_h
#define SQTile_h

#include <SDL2/SDL.h>
#include <stdio.h>
#include <stdlib.h>

extern const int SQ_TILE_WIDTH;
extern const int SQ_TILE_HEIGHT;
extern const struct SQTile SQ_TILE_EMPTY;
extern const struct SQTileset SQ_TILESET_EMPTY;

/// Struct representing a single tile in a grid
struct SQTile {
    /// Tile set from which to fetch this tile
    SDL_Texture *texture;
    /// The rect from the tile set corresponding to this tile
    SDL_Rect rect;
};

struct SQTile SQTileInit(SDL_Texture*, SDL_Rect);

struct SQTileset {
    int length;
    struct SQTile *tiles;
};

int SQTilesetInitFromTexture(struct SQTileset*, SDL_Texture*);

#endif /* SQTile_h */
