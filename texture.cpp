//
// Created by zxcroon on 17-6-19.
//

#include "texture.h"

GridTexture::GridTexture(Vec3b color1_, Vec3b color2_) : color1(color1_), color2(color2_), side(10) {}

GridTexture::GridTexture(ld side_, Vec3b color1_, Vec3b color2_) : color1(color1_), color2(color2_), side(side_) {}

Vec3b GridTexture::getColor(ld u, ld v) {
  int u1 = int(floor(u / side) + (u > 0 ? 0.5 : -0.5));
  int v1 = int(floor(v / side) + (v > 0 ? 0.5 : -0.5));
   return (u1 + v1) & 1 == 1 ? color1 : color2;
}
