//
// Created by zxcroon on 17-4-29.
//

#ifndef RAY_TRACING_BEZIER_H
#define RAY_TRACING_BEZIER_H

#include "basic_object.h"
#include "mesh.h"
#include "bounding.h"

typedef vector<vector<gPoint> > pointMatrix;

class BezierRotator : public BasicObject {
public:
  BezierRotator(vector<gPoint> pList_, ld xPivot_, ld zPivot_);
  bool intersection(Line l, gPoint &I, gVector &n);
private:
  vector<gPoint> pList;
  ld xPivot, zPivot;
};

class BezierSurface : public BasicObject {
public:
  BezierSurface(pointMatrix controllP_);
  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
private:
  static const ld TINY;

  int M, N;
  pointMatrix controllP;
  Bounding *bounding;

  Bounding *genBounding(ld umin, ld umax, ld vmin, ld vmax);
  gPoint value(ld u, ld v);
  gPoint derivativeU(ld u, ld v);
  gPoint derivativeV(ld u, ld v);
};

#endif //RAY_TRACING_BEZIER_H
