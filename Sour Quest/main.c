//
//  main.c
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include <SDL2/SDL.h>
#include <SDL2_image/SDL_image.h>
#include <stdio.h>
#include <stdlib.h>
#include "SQTile.h"
#include "SQTileMap.h"
#include "SQLevel.h"

const int targetX = 256;
const int targetY = 240;

SDL_Texture* createRenderTarget(SDL_Window* window, SDL_Renderer* renderer) {
    int windowW = 0;
    int windowH = 0;
    SDL_GetWindowSize(window, &windowW, &windowH);
    double aspectRatio = (double)windowW / windowH;
    
    int renderW;
    int renderH;
    if (aspectRatio < 1) {
        renderW = targetY * aspectRatio;
        renderH = targetY;
    } else {
        renderW = targetX;
        renderH = targetX / aspectRatio;
    }
    
    SDL_Texture* ret = SDL_CreateTexture
        (renderer,
         SDL_PIXELFORMAT_RGBA8888,
         SDL_TEXTUREACCESS_TARGET,
         renderW, renderH);
    return ret;
}

int main(int argc, const char * argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        SDL_Log("SDL could not be initialized: %s\n", SDL_GetError());
        return 1;
    }
    
    if (IMG_Init(IMG_INIT_PNG) != IMG_INIT_PNG) {
        SDL_Log("Could not initialize SDL Image: %s\n", IMG_GetError());
        goto exitError1;
    }
    
    SDL_Window *window = SDL_CreateWindow
        ("Sour Quest",
         SDL_WINDOWPOS_CENTERED,
         SDL_WINDOWPOS_CENTERED,
         640, 480,
         SDL_WINDOW_SHOWN | SDL_WINDOW_ALLOW_HIGHDPI);
    if (window == NULL) {
        SDL_Log("Could not create window: %s\n", SDL_GetError());
        goto exitError2;
    }
    
    SDL_Renderer *renderer = SDL_CreateRenderer
        (window, -1,
         SDL_RENDERER_ACCELERATED |
         SDL_RENDERER_PRESENTVSYNC);
    if (renderer == NULL) {
        SDL_Log("Could not create renderer: %s\n", SDL_GetError());
        goto exitError3;
    }
    
    
    SDL_Texture* renderTarget = createRenderTarget(window, renderer);
    if (!renderTarget) { goto exitError4; }
    
    // MARK: Load level
    struct SQArray levelTilesets;
    struct SQTileMap levelTilemap;
    struct SQLevel level;
    
    char* path = pathRelativeToResources("Level0.json");
    if(!path) { return 1; }
    if (SQLevel_ReadFromFile(&level, &levelTilemap, &levelTilesets, renderer, path)) {
        SDL_Log("Could not load level\n");
        goto exitError4;
    }
    sqFree(path);
    
    // MARK: Main loop
    int shouldClose = 0;
    while (!shouldClose) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                shouldClose = 1;
                break;
            }
        }
        
        SQLevel_Render(level, renderTarget);
        SDL_RenderCopy(renderer, renderTarget, NULL, NULL);
    }
    sqFree(levelTilemap.tiles);
    sqFree(levelTilesets.items);
    
    SDL_DestroyTexture(renderTarget);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    IMG_Quit();
    SDL_Quit();
    return 0;

exitError5:
    SDL_DestroyTexture(renderTarget);
exitError4:
    SDL_DestroyRenderer(renderer);
exitError3:
    SDL_DestroyWindow(window);
exitError2:
    IMG_Quit();
exitError1:
    SDL_Quit();
    return 1;
}
