//
//  SQTile.c
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQTile.h"

const int SQ_TILE_WIDTH = 16;
const int SQ_TILE_HEIGHT = 16;
const struct SQTile SQ_TILE_EMPTY = {NULL, {0, 0, 0, 0}};

struct SQTile SQTileInit(SDL_Texture* texture, SDL_Rect rect) {
    return (struct SQTile) {texture, rect};
}

SQTileset SQTileset_FromTexture(SDL_Texture* texture) {
    int w, h;
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0) { return NULL; }
    
    int tilesX = w / SQ_TILE_WIDTH;
    int tilesY = h / SQ_TILE_HEIGHT;
    int nTiles = tilesX * tilesY;
    SQTileset tiles = malloc((nTiles * sizeof(struct SQTile)));
    if (!tiles) { return NULL; }
    
    for (int i = 0; i < nTiles; i++) {
        int y = i / tilesX;
        int x = i % tilesX;
        tiles[i] = (struct SQTile) {texture, {x * SQ_TILE_WIDTH, y * SQ_TILE_HEIGHT, SQ_TILE_WIDTH, SQ_TILE_HEIGHT}};
    }
    
    return tiles;
}
