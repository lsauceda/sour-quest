//
//  SQEntity.h
//  Sour Quest
//
//  Created by Jorge on 8/16/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef SQEntity_h
#define SQEntity_h

#include <stdio.h>
#include "Utils.h"

struct SQEntity {
    struct SQEntity *parent;
    struct SQArray children;
    struct SQArray components;
};

struct SQEntity SQEntityInit(struct SQEntity*, struct SQArray, struct SQArray);

int SQEntityInit_Empty(struct SQEntity*, struct SQEntity*);

int SQEntity_SetParent(struct SQEntity*, struct SQEntity*);

int SQEntity_AddChild(struct SQEntity*, struct SQEntity*);

int SQEntity_AddComponent(struct SQEntity*, void *);

int SQEntity_RemoveComponent(struct SQEntity*, void *);

#endif /* SQEntity_h */
