//
// Created by zxcroon on 17-4-28.
//

#ifndef RAY_TRACING_GMATH_H
#define RAY_TRACING_GMATH_H

#include "base.h"

const ld PI = 3.14159265358979323846264;

ld det(ld a, ld b, ld c, ld d, ld e, ld f, ld g, ld h, ld i);

ld Bernstein(int i, int n, ld t);
ld derivativeOfBernstein(int i, int n, ld t);

#endif //RAY_TRACING_GMATH_H
