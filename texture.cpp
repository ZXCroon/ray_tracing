#include "texture.h"

GridTexture::GridTexture(Vec3b color1_, Vec3b color2_) : color1(color1_), color2(color2_), factor(10) {}

GridTexture::GridTexture(ld factor_, Vec3b color1_, Vec3b color2_) : color1(color1_), color2(color2_), factor(factor_) {}

Vec3b GridTexture::getColor(ld u, ld v) {
  int u1 = int(floor(u / factor) + (u > 0 ? 0.5 : -0.5));
  int v1 = int(floor(v / factor) + (v > 0 ? 0.5 : -0.5));
   return (u1 + v1) & 1 == 1 ? color1 : color2;
}

ImageTexture::ImageTexture(const string &name, ld factor_) : img(imread(name)), factor(factor_) {}

Vec3b ImageTexture::getColor(ld u, ld v) {
  if (u != u || v != v) {
    return Vec3b(0, 0, 0);
  }
  int u1 = abs(int(u / factor * img.cols + 0.5)), v1 = abs(int(v / factor * img.rows + 0.5));
  u1 %= 2 * img.cols;
  v1 %= 2 * img.rows;
  if (u1 >= img.cols) {
    u1 = 2 * img.cols - u1;
  }
  if (v1 >= img.rows) {
    v1 = 2 * img.rows - v1;
  }
  return img.at<Vec3b>(v1, u1);
}
