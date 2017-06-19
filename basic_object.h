//
// Created by zxcroon on 17-4-19.
//

#ifndef RAY_TRACING_BASIC_OBJECT_H
#define RAY_TRACING_BASIC_OBJECT_H

#include "geometry.h"

typedef vector<int> orderVector;

class BezierSurface;

class BasicObject {
public:
  virtual bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv) = 0;
  bool intersection0(Line l, gPoint &I, gVector &n);
};

class Closed : public BasicObject {

};

class Plane : public BasicObject {
public:
  gPoint P;
  gVector v;
  Plane(gPoint P_, gVector v_);
  Plane(gPoint P1, gPoint P2, gPoint P3);

  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
};

class Facet : public BasicObject {
public:
  Facet(vector<gPoint> vList_);
  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
private:
  vector<gPoint> vList;
  bool inside(gPoint P);
};

class Sphere : public Closed {
public:
  gPoint O;
  ld r;
  Sphere(gPoint O_, ld r_);

  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
};

class Box : public Closed {
public:
  Box(ld xmin_, ld xmax_, ld ymin_, ld ymax_, ld zmin_, ld zmax_);
  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
private:
  ld xmin, xmax, ymin, ymax, zmin, zmax;

  friend class BezierSurface;
};

class Tube : public Closed {
public:
  Tube(ld ymin_, ld ymax_, ld xPivot_, ld zPivot_, ld rmin_, ld rmax_);
  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
private:
  ld ymin, ymax, xPivot, zPivot, rmin, rmax;

  friend class BezierRotator;
  friend class Bounding;
};

#endif //RAY_TRACING_BASIC_OBJECT_H
