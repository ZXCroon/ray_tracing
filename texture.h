#ifndef RAY_TRACING_TEXTURE_H
#define RAY_TRACING_TEXTURE_H

#include "geometry.h"

class Texture {
public:
  virtual Vec3b getColor(ld u, ld v) = 0;
};

class GridTexture : public Texture {
public:
  GridTexture(Vec3b color1_=Vec3b(255, 255, 255), Vec3b color2_=Vec3b(0, 0, 0));
  GridTexture(ld side, Vec3b color1_=Vec3b(255, 255, 255), Vec3b color2_=Vec3b(0, 0, 0));
  Vec3b getColor(ld u, ld v);

private:
  Vec3b color1, color2;
  ld side;
};


#endif //RAY_TRACING_TEXTURE_H
