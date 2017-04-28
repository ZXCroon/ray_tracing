//
// Created by zxcroon on 17-4-28.
//

#include "gmath.h"

ld det(ld a, ld b, ld c, ld d, ld e, ld f, ld g, ld h, ld i) {
  return a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
}

