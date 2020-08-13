//
//  SQArray.h
//  Sour Quest
//
//  Created by Jorge on 8/8/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef SQArray_h
#define SQArray_h

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "Functions.h"

struct SQArray {
    size_t elementSize;
    int capacity;
    int length;
    void *items;
};

int SQArrayInit(struct SQArray* array, size_t elementSize, uint capacity, int length, void* data);

int SQArrayInit_Empty(struct SQArray* array, size_t elementSize);

int SQArrayInit_EmptyWithCapacity(struct SQArray* array, size_t elementSize, uint capacity);

int SQArrayInit_WithData(struct SQArray* array, size_t elementSize, int length, void* data);

int SQArray_ChangeCapacity(struct SQArray* array, uint newCapacity);

// TODO: Perhaps I should make an unsafe version without bounds checking
int SQArray_AppendData(struct SQArray* array, int length, const void *data);

int SQArray_AppendArray(struct SQArray* array, const struct SQArray source);

void SQArrayDeinit(struct SQArray * array);

#endif /* SQArray_h */
