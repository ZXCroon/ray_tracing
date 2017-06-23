//
// Created by zxcroon on 17-4-29.
//

#ifndef RAY_TRACING_BEZIER_H
#define RAY_TRACING_BEZIER_H

#include "basic_object.h"
#include "mesh.h"
#include "bounding.h"
#include <fstream>

typedef vector<vector<gPoint> > pointMatrix;

class BezierRotator : public BasicObject {
public:
  BezierRotator(vector<gPoint> pList_, ld xPivot_, ld zPivot_);
  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
  void writeToObj(string fileNmae);
private:
  static const ld TINY;

  int N;
  vector<gPoint> pList;
  ld xPivot, zPivot;
  Bounding *bounding;

  Bounding *genBounding(ld tmin, ld tmax);
  gPoint relativeValue(ld t);
  gPoint value(ld t, ld theta);
  void getDerivative(ld t, ld theta, gPoint &dT, gPoint &dTheta);
};

class BezierSurface : public BasicObject {
public:
  BezierSurface(pointMatrix controllP_);
  bool intersection(Line l, gPoint &I, gVector &n, UvParam &uv);
  void writeToObj(string fileName);
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
