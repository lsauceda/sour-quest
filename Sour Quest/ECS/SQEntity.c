//
//  SQEntity.c
//  Sour Quest
//
//  Created by Jorge on 8/16/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQEntity.h"

struct SQEntity SQEntityInit(struct SQEntity *parent, struct SQArray children, struct SQArray components) {
    return (struct SQEntity){parent, children, components};
}

int SQEntityInit_Empty(struct SQEntity* self, struct SQEntity* parent) {
    struct SQArray children;
    struct SQArray components;
    if (SQArrayInit_Empty(&children, sizeof(struct SQEntity*))) { return 1; }
    if (SQArrayInit_Empty(&components, sizeof(void*))) { goto returnWithError1; }
    *self = SQEntityInit(parent, children, components);
    return 0;
    
returnWithError1:
    sqFree(children.items);
    return 1;
}

int SQEntity_SetParent(struct SQEntity* self, struct SQEntity* parent) {
    if (SQArray_AppendData(&parent->children, 1, self)) { return 1; }
    SQArray_DeleteItem(&self->parent->children, &self);
    self->parent = parent;
    return 0;
}

int SQEntity_AddChild(struct SQEntity *self, struct SQEntity *child) {
    if (SQArray_AppendData(&self->children, 1, child)) { return 1; }
    SQArray_DeleteItem(&child->parent->children, &child);
    child->parent = self;
    return 0;
}

int SQEntity_AddComponent(struct SQEntity *self, void *component) {
    return SQArray_AppendData(&self->components, 1, component);
}

int SQEntity_RemoveComponent(struct SQEntity *self, void *component) {
    return SQArray_DeleteItem(&self->components, &component);
}
