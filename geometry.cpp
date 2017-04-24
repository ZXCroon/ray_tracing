//
// Created by zxcroon on 17-4-19.
//

#include "geometry.h"

gPoint::gPoint() : x(0), y(0), z(0) {}

gPoint::gPoint(ld x_, ld y_, ld z_) : x(x_), y(y_), z(z_) {}

gPoint::gPoint(Vec3b vec_) : x(vec_[0]), y(vec_[1]), z(vec_[2]) {}

ld &gPoint::operator[](int index) {
  switch (index) {
    case 0: return x;
    case 1: return y;
    case 2: return z;
  }
}

Line::Line() : P(gPoint()), v(gVector(1, 0, 0)) {}

Line::Line(gPoint P_, gVector v_) : P(P_), v(normalize(v_)) {}

std::ostream &operator<<(std::ostream &out, const gVector &v) {
  out << "(" << v.x << ", " << v.y << ", " << v.z << ")";
}

gVector operator+(gVector va, gVector vb) {
  return gVector(va.x + vb.x, va.y + vb.y, va.z + vb.z);
}

gVector operator-(gVector va, gVector vb) {
  return gVector(va.x - vb.x, va.y - vb.y, va.z - vb.z);
}

gVector operator-(gVector v) {
  return gVector(-v.x, -v.y, -v.z);
}

gVector operator*(gVector v, ld lambda) {
  return gVector(v.x * lambda, v.y * lambda, v.z * lambda);
}

gVector operator*(ld lambda, gVector v) {
  return v *lambda;
}

gVector operator/(gVector v, ld lambda) {
  return gVector(v.x / lambda, v.y / lambda, v.z / lambda);
}

ld dot(gVector va, gVector vb) {
  return va.x * vb.x + va.y * vb.y + va.z * vb.z;
}

gVector cross(gVector va, gVector vb) {
  return gVector(va.y * vb.z - va.z * vb.y,
                va.z * vb.x - va.x * vb.z,
                va.x * vb.y - va.y * vb.x);
}

ld norm(gVector v) {
  return sqrt(double(v.x * v.x + v.y * v.y + v.z * v.z));
}

ld cosAngle(gVector va, gVector vb) {
  return dot(va, vb) / (norm(va) * norm(vb));
}

ld angle(gVector va, gVector vb) {
  return acos(double(cosAngle(va, vb)));
}

ld distance(gPoint P1, gPoint P2) {
  return norm(P2 - P1);
}

ld distance(gPoint P, Line l) {
  return norm(cross(l.v, P - l.P)) / norm(l.v);
}

gVector normalize(gVector v) {
  return v / norm(v);
}

bool parellel(gVector v1, gVector v2) {
  return fabs(double(norm(cross(normalize(v1), normalize(v2))))) < eps;
}

bool perpendicular(gVector v1, gVector v2) {
  return fabs(double(dot(normalize(v1), normalize(v2)))) < eps;
}

gVector mirror(gVector v, gVector pivot) {
  return 2 * dot(v, pivot) * pivot / dot(pivot, pivot)  - v;
}


Vec3b operator*(ld k, Vec3b c) {
  return Vec3b(saturate_cast<uchar>(double(k * c[0])),
               saturate_cast<uchar>(double(k * c[1])),
               saturate_cast<uchar>(double(k * c[2])));
}

Vec3b operator*(gVector k, Vec3b c) {
  return Vec3b(saturate_cast<uchar>(double(k[0] * c[0])),
               saturate_cast<uchar>(double(k[1] * c[1])),
               saturate_cast<uchar>(double(k[2] * c[2])));
}