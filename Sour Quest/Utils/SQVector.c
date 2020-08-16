//
//  SQVector.c
//  Sour Quest
//
//  Created by Jorge on 8/15/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#include "SQVector.h"

struct SQVector SQ_VECTOR_ZERO  = { 0,  0};
struct SQVector SQ_VECTOR_LEFT  = {-1,  0};
struct SQVector SQ_VECTOR_RIGHT = { 1,  0};
struct SQVector SQ_VECTOR_DOWN  = { 0, -1};
struct SQVector SQ_VECTOR_UP    = { 0,  1};

struct SQVector SQVectorInit(const float x, const float y) {
    return (struct SQVector) {x, y};
}

struct SQVector SQVector_Invert(const struct SQVector v) {
    return (struct SQVector){-v.x, -v.y};
}

struct SQVector SQVector_Add(const struct SQVector v, const struct SQVector w) {
    return (struct SQVector){v.x + w.x, v.y + w.y};
}

struct SQVector SQVector_Subtract(const struct SQVector v, const struct SQVector w) {
    return (struct SQVector){v.x - w.x, v.y - w.y};
}

float SQVector_Dot(const struct SQVector v, const struct SQVector w) {
    return (v.x * w.x) + (v.y * w.y);
}
