//
//  SQArray.c
//  Sour Quest
//
//  Created by Jorge on 8/8/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQArray.h"

int SQArrayInit(struct SQArray* array, size_t elementSize, uint capacity, int length, void* data) {
    assert(length >= 0);
    void *newData = malloc(elementSize * capacity);
    if (!newData) { return 1; }
    if (length && data) {
        memcpy(newData, data, elementSize * length);
    }
    *array = (struct SQArray){elementSize, capacity, length, newData};
    return 0;
}

int SQArrayInit_Empty(struct SQArray* array, size_t elementSize) {
    return SQArrayInit(array, elementSize, 0, 0, NULL);
}

int SQArrayInit_EmptyWithCapacity(struct SQArray* array, size_t elementSize, uint capacity) {
    return SQArrayInit(array, elementSize, capacity, 0, NULL);
}

int SQArrayInit_WithData(struct SQArray* array, size_t elementSize, int length, void* data) {
    uint capacity = nextPow2(length);
    return SQArrayInit(array, elementSize, capacity, length, data);
}

int SQArray_ChangeCapacity(struct SQArray* array, uint newCapacity) {
    uint pow2Capacity = nextPow2(newCapacity);
    void *newPointer = realloc(array->items, pow2Capacity * array->elementSize);
    if (!newPointer) { return 1; }
    array->items = newPointer;
    return 0;
}

int SQArray_AppendData(struct SQArray* array, int length, const void *data) {
    assert(length >= 0);
    int remainingSpaces = array->capacity - array->length;
    if (remainingSpaces < length) {
        if (SQArray_ChangeCapacity(array, array->length + length)) {
            return 1;
        }
    }
    // FIXME: Is char guaranteed to be 1 byte?
    void *dst = ((char*)array->items + (array->length * array->elementSize));
    if (length && data) {
        memcpy(dst, data, length * array->elementSize);
        array->length += length;
    }
    
    return 0;
}

int SQArray_AppendArray(struct SQArray* array, const struct SQArray source) {
    return SQArray_AppendData(array, source.length, source.items);
}

void SQArrayDeinit(struct SQArray * array) {
    array->elementSize = 0;
    array->capacity = 0;
    array->length = 0;
    sqFree(array->items);
}
