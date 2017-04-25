//
// Created by zxcroon on 17-4-20.
//

#ifndef RAY_TRACING_SCENE_H
#define RAY_TRACING_SCENE_H

#include "base.h"
#include "object.h"
#include "light.h"

class Scene {
public:
  Scene(int height_, int width_, int zScreen_, gPoint aperture_);
  void addObject(Object *object_);
  void setAmbientLight(AmbientLight *ambientLight_);
  void addLight(Light *light_);
  Mat render();

private:
  int width, height, zScreen;
  gPoint aperture;
  vector<Object *> objects;
  AmbientLight *ambientLight;
  vector<Light *> lights;

  Vec3b rayTracing(Line l, ld decay);
};

#endif //RAY_TRACING_SCENE_H
