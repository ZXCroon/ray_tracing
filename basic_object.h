//
// Created by zxcroon on 17-4-19.
//

#ifndef RAY_TRACING_BASIC_OBJECT_H
#define RAY_TRACING_BASIC_OBJECT_H

#include "geometry.h"

class BasicObject {
public:
  virtual bool intersection(Line l, gPoint &I, gVector &n) = 0;
};

class Plane : public BasicObject {
public:
  gPoint P;
  gVector v;
  Plane(gPoint P_, gVector v_);
  Plane(gPoint P1, gPoint P2, gPoint P3);

  bool intersection(Line l, gPoint &I, gVector &n);
};

class Sphere : public BasicObject {
public:
  gPoint O;
  ld r;
  Sphere(gPoint O_, ld r_);

  bool intersection(Line l, gPoint &I, gVector &n);
};

#endif //RAY_TRACING_BASIC_OBJECT_H
