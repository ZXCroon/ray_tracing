//
// Created by zxcroon on 17-4-20.
//

#ifndef RAY_TRACING_SCENE_H
#define RAY_TRACING_SCENE_H

#include "base.h"
#include "object.h"
#include "light.h"
#include <fstream>

class Scene {
public:
  Scene(int width, int height, Plane screen_, gVector pivot1_, gPoint aperture_, int radius_=1);
  void addObject(Object *object_);
  void setAmbientLight(AmbientLight *ambientLight_);
  void addLight(Light *light_);
  void setFocalPlaneDist(ld d);
  void superSamplingOn(bool b);
  Mat render();

private:
  int width, height, radius;
  Plane screen, focalPlane;
  gVector pivot1, pivot2;
  gPoint aperture;
  vector<Object *> objects;
  AmbientLight *ambientLight;
  vector<Light *> lights;
  bool ss;

  Vec3b renderOnce(gPoint startingPoint);
  gPoint calcStarting(ld x1, ld x2);
  Vec3b rayTracing(Line l, ld decay);
};

#endif //RAY_TRACING_SCENE_H
