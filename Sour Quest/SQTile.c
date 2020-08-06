//
//  SQTile.c
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQTile.h"


struct SQTile _SQ_TILE_ARRAY[] = {{NULL, {0, 0, 0, 0}}};
const int SQ_TILE_WIDTH = 16;
const int SQ_TILE_HEIGHT = 16;
const struct SQTile SQ_TILE_EMPTY = {NULL, {0, 0, 0, 0}};
const struct SQTileset SQ_TILESET_EMPTY = {1, _SQ_TILE_ARRAY};

struct SQTile SQTileInit(SDL_Texture* texture, SDL_Rect rect) {
    return (struct SQTile) {texture, rect};
}

int SQTilesetInitFromTexture(struct SQTileset *tileset, SDL_Texture* texture) {
    int w, h;
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0) { return 1; }
    
    int tilesX = w / SQ_TILE_WIDTH;
    int tilesY = h / SQ_TILE_HEIGHT;
    int nTiles = tilesX * tilesY;
    struct SQTileset ret;
    ret.length = nTiles;
    ret.tiles = malloc(nTiles * sizeof(struct SQTile));
    if (!ret.tiles) { return 1; }
    
    for (int i = 0; i < nTiles; i++) {
        div_t coordinates = div(i, tilesX);
        int y = coordinates.quot;
        int x = coordinates.rem;
        ret.tiles[i] = (struct SQTile) {texture, {x * SQ_TILE_WIDTH, y * SQ_TILE_HEIGHT, SQ_TILE_WIDTH, SQ_TILE_HEIGHT}};
    }
    *tileset = ret;
    return 0;
}
