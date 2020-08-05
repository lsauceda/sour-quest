//
//  SQLevel.c
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQLevel.h"

struct SQLevel SQLevelInit(SDL_Renderer* renderer, struct SQTileMap tileMap) {
    return (struct SQLevel) {renderer, tileMap};
}

int SQLevel_ReadFromFile(struct SQLevel* level, struct SQTileMap* tilemap, SQTileset** tilesets, SDL_Renderer* renderer, const char* fileName) {
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
    SQTileset* deserializedTilesets = malloc((tilesetsCount + 1) * sizeof(SQTileset));
    if (!deserializedTilesets) { goto returnWithError1; }
    // First tileset is always empty tileset
    deserializedTilesets[0] = (SQTileset) &SQ_TILE_EMPTY;
    int i = 1;
    cJSON *tilesetJSON = NULL;
    cJSON_ArrayForEach(tilesetJSON, tilesetsJSON) {
        if (!cJSON_IsString(tilesetJSON)) { goto setEmptyTileset; }
        const char* textureFile = cJSON_GetStringValue(tilesetJSON);
        SDL_Texture *texture = IMG_LoadTexture(renderer, textureFile);
        if (!texture) { goto setEmptyTileset; }
        
        SQTileset tileset = SQTileset_FromTexture(texture);
        if (!tileset) { goto setEmptyTileset; }
        deserializedTilesets[i] = tileset;
        i++;
        continue;
        
    setEmptyTileset:
        deserializedTilesets[i] = (SQTileset) &SQ_TILE_EMPTY;
        i++;
    }
    
    // Deserialize tilemap
    struct SQTileMap deserializedMap;
    deserializedMap.width = (int)cJSON_GetNumberValue(tilemapWidthJSON);
    deserializedMap.height = (int)cJSON_GetNumberValue(tilemapHeightJSON);
    int tileArraySize = cJSON_GetArraySize(tilesJSON);
    deserializedMap.tiles = malloc(tileArraySize * sizeof(struct SQTile));
    if (!deserializedMap.tiles) { goto returnWithError2; }
    i = 0;
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
        deserializedMap.tiles[i] = deserializedTilesets[tilesetIndex][tileIndex];
        i++;
        continue;
        
    setEmptyTile:
        deserializedMap.tiles[i] = SQ_TILE_EMPTY;
        i++;
    }
    
    struct SQLevel deserializedLevel = SQLevelInit(renderer, deserializedMap);
    
    // Set return values
    *tilesets = deserializedTilesets;
    *tilemap = deserializedMap;
    *level = deserializedLevel;

    //Cleanup
    cJSON_Delete(levelJSON);
    return 0;

returnWithError2:
    sqFree(deserializedTilesets);
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
void SQLevel_Render(struct SQLevel level) {
    SDL_RenderClear(level.renderer);
    int nTiles = level.tilemap.width * level.tilemap.height;
    for (int i = 0; i < nTiles; i++) {
        struct SQTile tile = level.tilemap.tiles[i];
        if (tile.texture == NULL) { continue; }
        
        int y = i / level.tilemap.width;
        int x = i % level.tilemap.width;
        // FIXME: render at original resolution and stretch to match screen
        SDL_Rect destination = {x * SQ_TILE_WIDTH * 4, y * SQ_TILE_HEIGHT * 4, SQ_TILE_WIDTH * 4, SQ_TILE_HEIGHT * 4};
        SDL_RenderCopy(level.renderer, tile.texture, &tile.rect, &destination);
    }
    SDL_RenderPresent(level.renderer);
}
