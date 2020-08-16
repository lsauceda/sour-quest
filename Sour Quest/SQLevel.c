//
//  SQLevel.c
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQLevel.h"

struct SQLevel SQLevelInit(SDL_Renderer* renderer, struct SQTileMap tileMap, struct SQVector cameraPosition) {
    return (struct SQLevel) {renderer, tileMap, cameraPosition};
}

// TODO: Allow camera position to be specifieds
int SQLevel_ReadFromFile(struct SQLevel* level, struct SQTileMap* tilemap, struct SQArray *tilesets, SDL_Renderer* renderer, const char* fileName) {
    char* text = fileToString(fileName);
    
    // Parse top level JSON object
    cJSON *levelJSON = cJSON_Parse(text);
    sqFree(text);
    if (!levelJSON) { goto returnWithError1; }
    
    // Get values from JSON
    cJSON *tilesetsJSON = cJSON_GetObjectItemCaseSensitive(levelJSON, "tilesets");
    cJSON *tilemapJSON = cJSON_GetObjectItemCaseSensitive(levelJSON, "tilemap");
    if (!cJSON_IsArray(tilesetsJSON) || !cJSON_IsObject(tilemapJSON)) {
        goto returnWithError1;
    }
    cJSON *tilemapWidthJSON = cJSON_GetObjectItemCaseSensitive(tilemapJSON, "width");
    cJSON *tilemapHeightJSON = cJSON_GetObjectItemCaseSensitive(tilemapJSON, "height");
    cJSON *tilesJSON = cJSON_GetObjectItemCaseSensitive(tilemapJSON, "tiles");
    if (!cJSON_IsNumber(tilemapWidthJSON) || !cJSON_IsNumber(tilemapHeightJSON) || !cJSON_IsArray(tilesJSON)) {
        goto returnWithError1;
    }
    
    // Deserialize tilesets
    int tilesetsCount = cJSON_GetArraySize(tilesetsJSON);
    struct SQArray deserializedTilesets;
    if (SQArrayInit_EmptyWithCapacity(&deserializedTilesets, sizeof(SQTileset), tilesetsCount)) {
        goto returnWithError1;
    }
    // First tileset is always empty tileset
    if (SQArray_AppendData(&deserializedTilesets, 1, &SQ_TILESET_EMPTY)) {
        goto returnWithError2;
    }
    cJSON *tilesetJSON = NULL;
    cJSON_ArrayForEach(tilesetJSON, tilesetsJSON) {
        if (!cJSON_IsString(tilesetJSON)) { goto setEmptyTileset; }
        const char* textureFile = cJSON_GetStringValue(tilesetJSON);
        SDL_Texture *texture = IMG_LoadTexture(renderer, textureFile);
        if (!texture) { goto setEmptyTileset; }
        
        SQTileset tileset;
        if (SQTilesetInit_FromTexture(&tileset, texture)) { goto setEmptyTileset; }
        if (SQArray_AppendData(&deserializedTilesets, 1, &tileset)) {
            goto setEmptyTileset;
        }

        continue;
    setEmptyTileset:
        SQArray_AppendData(&deserializedTilesets, 1, &SQ_TILESET_EMPTY);
    }
    
    // Deserialize tilemap
    struct SQTileMap deserializedMap;
    deserializedMap.width = (int)cJSON_GetNumberValue(tilemapWidthJSON);
    deserializedMap.height = (int)cJSON_GetNumberValue(tilemapHeightJSON);
    if (deserializedMap.width < 0 || deserializedMap.height < 0) { goto returnWithError2; }
    int tileArraySize = cJSON_GetArraySize(tilesJSON);
    deserializedMap.tiles = malloc(tileArraySize * sizeof(struct SQTile));
    if (!deserializedMap.tiles) { goto returnWithError2; }
    int i = 0;
    cJSON *tileJSON = NULL;
    cJSON_ArrayForEach(tileJSON, tilesJSON) {
        if (!cJSON_IsArray(tileJSON)) { goto setEmptyTile; }
        cJSON *tilesetIndexJSON = cJSON_GetArrayItem(tileJSON, 0);
        cJSON *tileIndexJSON = cJSON_GetArrayItem(tileJSON, 1);
        if (!cJSON_IsNumber(tilesetIndexJSON) || !cJSON_IsNumber(tileIndexJSON)) {
            goto setEmptyTile;
        }
        int tilesetIndex = (int)cJSON_GetNumberValue(tilesetIndexJSON);
        int tileIndex = (int)cJSON_GetNumberValue(tileIndexJSON);
        if (tilesetIndex < 0 || tileIndex < 0) { goto setEmptyTile; }
        if (tilesetIndex >= deserializedTilesets.length) { goto returnWithError2; }
        SQTileset _tileset = ((SQTileset*)deserializedTilesets.items)[tilesetIndex];
        if (tileIndex >= _tileset.length) { goto returnWithError2; }
        struct SQTile _tile = ((struct SQTile*)_tileset.items)[tileIndex];
        deserializedMap.tiles[i] = _tile;
        i++;
        
        continue;
    setEmptyTile:
        deserializedMap.tiles[i] = SQ_TILE_EMPTY;
        i++;
    }
    
    // FIXME: Camera position is always zero (should be read from file)
    struct SQLevel deserializedLevel = SQLevelInit(renderer, deserializedMap, SQ_VECTOR_ZERO);
    
    // Set return values
    *tilesets = deserializedTilesets;
    *tilemap = deserializedMap;
    *level = deserializedLevel;

    //Cleanup
    cJSON_Delete(levelJSON);
    return 0;

returnWithError2:
    sqFree(deserializedTilesets.items);
returnWithError1:
    cJSON_Delete(levelJSON);
    return 1;
}

int SQLevel_SetBackgroundColor(struct SQLevel* level, SDL_Color color) {
    return SDL_SetRenderDrawColor
        (level->renderer,
         color.r,
         color.g,
         color.b,
         color.a);
}

// TODO: Only render visible tiles
void SQLevel_Render(struct SQLevel level, SDL_Texture *target) {
    SDL_SetRenderTarget(level.renderer, target);
    SDL_RenderClear(level.renderer);
    int nTiles = level.tilemap.width * level.tilemap.height;
    for (int i = 0; i < nTiles; i++) {
        struct SQTile tile = level.tilemap.tiles[i];
        if (tile.texture == NULL) { continue; }

        div_t coordinates = div(i, level.tilemap.width);
        int x = coordinates.rem * SQ_TILE_WIDTH;
        int y = coordinates.quot * SQ_TILE_HEIGHT;
        struct SQVector tilePosition = SQVectorInit(x, y);
        tilePosition = SQVector_Subtract(tilePosition, level.cameraPosition);
        
        // FIXME: Image is not getting stretched appropriately
        SDL_Rect destination = {tilePosition.x, tilePosition.y, SQ_TILE_WIDTH, SQ_TILE_HEIGHT};
        SDL_RenderCopy(level.renderer, tile.texture, &tile.rect, &destination);
    }
    SDL_RenderPresent(level.renderer);
    SDL_SetRenderTarget(level.renderer, NULL);
}
