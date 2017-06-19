//
// Created by zxcroon on 17-4-19.
//

#include "basic_object.h"

bool BasicObject::intersection0(Line l, gPoint &I, gVector &n) {
  UvParam uv;
  return intersection(l, I, n, uv);
}


Plane::Plane(gPoint P_, gVector v_) : P(P_), v(normalize(v_)) {}

Plane::Plane(gPoint P1, gPoint P2, gPoint P3) : P(P1), v(normalize(cross(P2 - P1, P3 - P2))) {}

bool Plane::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  if (perpendicular(this->v, l.v)) {
    return false;
  }
  ld len = dot(this->v, this->P - l.P) / dot(this->v, l.v);
  if (len < 0) {
    return false;
  }
  I = l.P + l.v * len;
  n = this->v;

  gVector pivot;
  if (n.x < eps && n.y < eps) {
    pivot = gVector(1, 0, 0);
  }
  else {
    pivot = normalize(gVector(n.y, -n.x, 0));
  }
  gVector shift = I - this->P;
  uv = UvParam(dot(shift, pivot) / 5, norm(cross(shift, pivot)) / 5);
  return true;
}


Sphere::Sphere(gPoint O_, ld r_) : O(O_), r(r_) {}

bool Sphere::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
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

bool Facet::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  Plane plane(vList[0], vList[1], vList[2]);
  if (!plane.intersection(l, I, n, uv)) {
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


Box::Box(ld xmin_, ld xmax_, ld ymin_, ld ymax_, ld zmin_, ld zmax_) :
        xmin(xmin_), xmax(xmax_), ymin(ymin_), ymax(ymax_), zmin(zmin_), zmax(zmax_) {}

bool Box::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  if (l.P.x < xmin && l.v.x <= 0 || l.P.x > xmax && l.v.x >= 0) {
    return false;
  }
  if (l.P.y < ymin && l.v.y <= 0 || l.P.y > ymax && l.v.y >= 0) {
    return false;
  }
  if (l.P.z < zmin && l.v.z <= 0 || l.P.z > zmax && l.v.z >= 0) {
    return false;
  }

  ld x0, y0, z0;
  if (l.P.x < xmin  && l.v.x > 0 || l.P.x >= xmin && l.P.x <= xmax && l.v.x < 0) {
    x0 = xmin;
  }
  if (l.P.x > xmax  && l.v.x < 0 || l.P.x >= xmin && l.P.x <= xmax && l.v.x > 0) {
    x0 = xmax;
  }
  if (l.P.y < ymin  && l.v.y > 0 || l.P.y >= ymin && l.P.y <= ymax && l.v.y < 0) {
    y0 = ymin;
  }
  if (l.P.y > ymax  && l.v.y < 0 || l.P.y >= ymin && l.P.y <= ymax && l.v.y > 0) {
    y0 = ymax;
  }
  if (l.P.z < zmin  && l.v.z > 0 || l.P.z >= zmin && l.P.z <= zmax && l.v.z < 0) {
    z0 = zmin;
  }
  if (l.P.z > zmax  && l.v.z < 0 || l.P.z >= zmin && l.P.z <= zmax && l.v.z > 0) {
    z0 = zmax;
  }

  ld tx = (x0 - l.P.x) / l.v.x;
  ld ty = (y0 - l.P.y) / l.v.y;
  ld tz = (z0 - l.P.z) / l.v.z;
  bool its = false;
  ld nowt;

  if (!its || tx < nowt) {
    gPoint I1 = l.P + l.v * tx;
    if (I1.y >= ymin && I1.y <= ymax && I1.z >= zmin && I1.z <= zmax) {
      its = true;
      nowt = tx;
      I = I1;
      if (fabs(x0 - xmin) < eps) {
        n = gVector(-1, 0, 0);
      }
      else {
        n = gVector(1, 0, 0);
      }
    }
  }

  if (!its || ty < nowt) {
    gPoint I1 = l.P + l.v * ty;
    if (I1.z >= zmin && I1.z <= zmax && I1.x >= xmin && I1.x <= xmax) {
      its = true;
      nowt = ty;
      I = I1;
      if (fabs(y0 - ymin) < eps) {
        n = gVector(0, -1, 0);
      }
      else {
        n = gVector(0, 1, 0);
      }
    }
  }

  if (!its || tz < nowt) {
    gPoint I1 = l.P + l.v * tz;
    if (I1.x >= xmin && I1.x <= xmax && I1.y >= ymin && I1.y <= ymax) {
      its = true;
      nowt = tz;
      I = I1;
      if (fabs(z0 - zmin) < eps) {
        n = gVector(0, 0, -1);
      }
      else {
        n = gVector(0, 0, 1);
      }
    }
  }

  return its;
}


Tube::Tube(ld ymin_, ld ymax_, ld xPivot_, ld zPivot_, ld rmin_, ld rmax_) :
        ymin(ymin_), ymax(ymax_), xPivot(xPivot_), zPivot(zPivot_), rmin(rmin_), rmax(rmax_) {}

bool Tube::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  if (l.P.y < ymin && l.v.y <= 0 || l.P.y > ymax && l.v.y >= 0) {
    return false;
  }

  Line castL(l.P, gVector(l.v.x, 0, l.v.z));
  ld S = dot(gPoint(xPivot, castL.P.y, zPivot) - castL.P, castL.v);
  if (S <= 0) {
    return false;
  }

  ld d = distance(gPoint(xPivot, l.P.y, zPivot), castL);
  if (d > rmax) {
    return false;
  }

  ld ty, trmin, trmax;
  ld y0;
  if (l.P.y < ymin && l.v.y > 0 || l.P.y >= ymin && l.P.y <= ymax && l.v.y < 0) {
    y0 = ymin;
  }
  if (l.P.y > ymax && l.v.y < 0 || l.P.y >= ymin && l.P.y <= ymax && l.v.y > 0) {
    y0 = ymax;
  }
  ty = (y0 - l.P.y) / l.v.y;

  trmin = S + sqrt(rmin * rmin - d * d);
  trmin /= norm(gVector(l.v.x, 0, l.v.z));

  trmax = S;
  bool outside = (distance(gPoint(xPivot, l.P.y, zPivot), l.P) > rmax);
  if (outside) {
    trmax -= sqrt(rmax * rmax - d * d);
  }
  else {
    trmax += sqrt(rmax * rmax - d * d);
  }
  trmax /= norm(gVector(l.v.x, 0, l.v.z));

  bool its = false;
  ld nowt;

  if (!its || ty < nowt) {
    gPoint I1 = l.P + l.v * ty;
    ld dist = distance(I1, gPoint(xPivot, I1.y, zPivot));
    if (dist >= rmin && dist <= rmax) {
      its = true;
      nowt = ty;
      I = I1;
      if (fabs(y0 - ymin) < eps) {
        n = gVector(0, -1, 0);
      }
      else {
        n = gVector(0, 1, 0);
      }
    }
  }

  if (!its || trmin < nowt) {
    gPoint I1 = l.P + l.v * trmin;
    if (I1.y >= ymin && I1.y <= ymax) {
      its = true;
      nowt = trmin;
      I = I1;
      n = normalize(I1 - gPoint(xPivot, I1.y, zPivot));
    }
  }

  if (!its || trmax < nowt) {
    gPoint I1 = l.P + l.v * trmax;
    if (I1.y >= ymin && I1.y <= ymax) {
      its = true;
      nowt = trmax;
      I = I1;
      n = normalize(gPoint(xPivot, I1.y, zPivot) - I1);
    }
  }

  return its;
}