//
// Created by zxcroon on 17-4-19.
//

#ifndef RAY_TRACING_BASIC_OBJECT_H
#define RAY_TRACING_BASIC_OBJECT_H

#include "geometry.h"

typedef vector<int> orderVector;

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

class Facet : public BasicObject {
public:
  Facet(vector<gPoint> vList_);
  bool intersection(Line l, gPoint &I, gVector &n);
private:
  vector<gPoint> vList;
  bool inside(gPoint P);
};

class Sphere : public BasicObject {
public:
  gPoint O;
  ld r;
  Sphere(gPoint O_, ld r_);

  bool intersection(Line l, gPoint &I, gVector &n);
};

class Box : public BasicObject {
public:
  Box(ld xmin_, ld xmax_, ld ymin_, ld ymax_, ld zmin_, ld zmax_);
  bool intersection(Line l, gPoint &I, gVector &n);
private:
  ld xmin, xmax, ymin, ymax, zmin, zmax;
};

class Tube : public BasicObject {
public:
  Tube(ld ymin_, ld ymax_, ld xPivot_, ld zPivot_, ld rmin_, ld rmax_);
  bool intersection(Line l, gPoint &I, gVector &n);
private:
  ld ymin, ymax, xPivot, zPivot, rmin, rmax;
};

#endif //RAY_TRACING_BASIC_OBJECT_H
