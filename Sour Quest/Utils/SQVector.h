//
//  SQVector.h
//  Sour Quest
//
//  Created by Jorge on 8/15/20.
//  Copyright © 2020 Sirius Game Labs. All rights reserved.
//

#ifndef SQVector_h
#define SQVector_h

#include <stdio.h>

// I hope all of this gets vectorized!

struct SQVector {
    float x;
    float y;
};

extern struct SQVector SQ_VECTOR_ZERO;
extern struct SQVector SQ_VECTOR_UP;
extern struct SQVector SQ_VECTOR_DOWN;
extern struct SQVector SQ_VECTOR_LEFT;
extern struct SQVector SQ_VECTOR_RIGHT;

struct SQVector SQVectorInit(const float, const float);
struct SQVector SQVector_Invert(const struct SQVector);
struct SQVector SQVector_Add(const struct SQVector, const struct SQVector);
struct SQVector SQVector_Subtract(const struct SQVector, const struct SQVector);
float SQVector_Dot(const struct SQVector, const struct SQVector);

#endif /* SQVector_h */
