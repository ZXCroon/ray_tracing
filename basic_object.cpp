//
// Created by zxcroon on 17-4-19.
//

#include "basic_object.h"

Plane::Plane(gPoint P_, gVector v_) : P(P_), v(normalize(v_)) {}

Plane::Plane(gPoint P1, gPoint P2, gPoint P3) : P(P1), v(normalize(cross(P2 - P1, P3 - P1))) {}

bool Plane::intersection(Line l, gPoint &I, gVector &n) {
  if (perpendicular(this->v, l.v)) {
    return false;
  }
  ld len = dot(this->v, this->P - l.P) / dot(this->v, l.v);
  if (len < 0) {
    return false;
  }
  I = l.P + l.v * len;
  if (dot(this->v, l.v) > 0) {
    n = -this->v;
  }
  else {
    n = this->v;
  }
  return true;
}

Sphere::Sphere(gPoint O_, ld r_) : O(O_), r(r_) {}

bool Sphere::intersection(Line l, gPoint &I, gVector &n) {
  ld d = distance(this->O, l);
  if (d > this->r) {
    return false;
  }

  ld S = dot(this->O - l.P, l.v);
  if (S < 0) {
    return false;
  }
  ld len = S;

  bool outside = distance(this->O, l.P) > this->r;
  if (outside) {
    len -= sqrt(double(this->r * this->r - d * d));
  }
  else {
    len += sqrt(double(this->r * this->r - d * d));
  }
  I = l.P + l.v * len;
  n = normalize(I - this->O);
  if (!outside) {
    n = -n;
  }
  return true;
}
