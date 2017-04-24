//
// Created by zxcroon on 17-4-20.
//

#ifndef RAY_TRACING_LIGHT_H
#define RAY_TRACING_LIGHT_H

#include "geometry.h"

class Light {
public:
  virtual void getLight(gPoint point, Vec3b &inten, gVector &direction) = 0;
  virtual gPoint getPosition() = 0;
  Vec3b intensity;
};

class PointLight : public Light {
public:
  PointLight(gPoint position_, Vec3b intensity_);
  gPoint getPosition();
  void getLight(gPoint point, Vec3b &inten, gVector &direction);

private:
  gPoint position;
  Vec3b intensity;
};

class SpotLight : public Light {

};

class DirectionalLight : public Light {

};

class AmbientLight {
public:
  AmbientLight(Vec3b intensity_);
  Vec3b getIntensity();
private:
  Vec3b intensity;
};

#endif //RAY_TRACING_LIGHT_H
