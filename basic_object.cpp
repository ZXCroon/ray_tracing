//
// Created by zxcroon on 17-4-19.
//

#include "basic_object.h"

Plane::Plane(gPoint P_, gVector v_) : P(P_), v(normalize(v_)) {}

Plane::Plane(gPoint P1, gPoint P2, gPoint P3) : P(P1), v(normalize(cross(P2 - P1, P3 - P2))) {}

bool Plane::intersection(Line l, gPoint &I, gVector &n) {
  if (perpendicular(this->v, l.v)) {
    return false;
  }
  ld len = dot(this->v, this->P - l.P) / dot(this->v, l.v);
  if (len < 0) {
    return false;
  }
  I = l.P + l.v * len;
  n = this->v;
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
  return true;
}


Facet::Facet(vector<gPoint> vList_) {
  assert(coplanar(vList_));
  vList = vList_;
}

bool Facet::intersection(Line l, gPoint &I, gVector &n) {
  Plane plane(vList[0], vList[1], vList[2]);
  if (!plane.intersection(l, I, n)) {
    return false;
  }
  return inside(I);
}

bool Facet::inside(gPoint P) {
  gVector pivot = normalize(vList[0] - P + vList[1] - P);
  gVector pivot2 = normalize(cross(vList[0] - P, vList[1] - P));
  int *area = new int[vList.size()];
  for (int i = 0; i < vList.size(); ++i) {
    if (dot(pivot2, cross(pivot, vList[i] - P)) >= 0) {
      if (dot(pivot, vList[i] - P) >= 0) {
        area[i] = 0;
      }
      else {
        area[i] = 1;
      }
    }
    else {
      if (dot(pivot, vList[i] - P) >= 0) {
        area[i] = 3;
      }
      else {
        area[i] = 2;
      }
    }
  }

  int cnt = 0;
  for (int i = 0; i < vList.size(); ++i) {
    int j = (i + 1) % vList.size();
    if (area[i] == area[j]) {
      continue;
    }
    if (area[j] == (area[i] + 1) % 4) {
      ++cnt;
      continue;
    }
    if (area[i] == (area[j] + 1) % 4) {
      --cnt;
      continue;
    }

    if (dot(pivot2, cross(vList[i] - P, vList[j] - P)) > 0) {
      cnt += 2;
    }
    else {
      cnt -= 2;
    }
  }

  return cnt == 4;
}
