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

class Mesh : public BasicObject {
public:
  Mesh(vector<gPoint> vList_, vector<orderVector> fList_);
  bool intersection(Line l, gPoint &I, gVector &n);

private:
  vector<gPoint> vList;
  vector <orderVector> fList;
};

#endif //RAY_TRACING_BASIC_OBJECT_H
