//
//  SQTileMap.c
//  SourQuest
//
//  Created by Jorge on 7/26/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQTileMap.h"


struct SQTileMap SQTileMapInit(int width, int height, struct SQTile *tiles) {
    return (struct SQTileMap) {width, height, tiles};
}
