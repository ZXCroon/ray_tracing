//
// Created by zxcroon on 17-4-19.
//

#ifndef RAY_TRACING_GEOMETRY_H
#define RAY_TRACING_GEOMETRY_H

#include "base.h"
#include "gmath.h"

typedef std::pair<ld, ld> UvParam;

class gPoint {
public:
  ld x, y, z;
  gPoint();
  gPoint(ld x_, ld y_, ld z_);
  gPoint(Vec3b vec_);

  ld &operator[](int index);
};

typedef gPoint gVector;

class Line {
public:
  gPoint P;
  gVector v;
  Line();
  Line(gPoint P_, gVector v_);
};

std::ostream &operator<<(std::ostream &out, const gVector &v);

gVector operator+(gVector va, gVector vb);
gVector operator-(gVector va, gVector vb);
gVector operator-(gVector v);
gVector operator*(gVector v, ld lambda);
gVector operator*(ld lambda, gVector v);
gVector operator/(gVector v, ld lambda);
ld dot(gVector va, gVector vb);
gVector cross(gVector va, gVector vb);
ld norm(gVector v);
ld cosAngle(gVector va, gVector vb);
ld angle(gVector va, gVector vb);
ld distance(gPoint P1, gPoint P2);
ld distance(gPoint P, Line l);
gVector normalize(gVector v);
bool parellel(gVector v1, gVector v2);
bool perpendicular(gVector v1, gVector v2);
gVector mirror(gVector v, gVector pivot);
bool coplanar(vector<gPoint> vList);

Vec3b operator*(ld k, Vec3b v);
Vec3b operator*(gVector k, Vec3b v);
gVector operator*(gVector va, gVector vb);

#endif //RAY_TRACING_GEOMETRY_H
