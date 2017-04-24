//
// Created by zxcroon on 17-4-20.
//

#include "object.h"

void Object::incidence(Line l) {
  inl = l;
  calc();
}

bool Object::beIntersectant() {
  return intersectant;
}

gPoint Object::getIntersection() {
  return intersection;
}

gVector Object::getNormalVector() {
  return normalVector;
}


SimpleObject::SimpleObject(BasicObject *bobj_, Vec3b color_) :
        bobj(bobj_), color(color_),
        kD(gVector(color_) / 255),
        kA(gVector(color_) / 255),
        kS(0.4),
        reflectance(0),
        transmittance(0),
        refractivity(1) {
}

void SimpleObject::setKD(gVector kD_) {
  kD = kD_;
}

void SimpleObject::setKD(ld kDb_, ld kDg_, ld kDr_) {
  kD = gVector(kDb_, kDg_, kDr_);
}

void SimpleObject::setKA(ld kAb_, ld kAg_, ld kAr_) {
  kA = gVector(kAb_, kAg_, kAr_);
}

void SimpleObject::setKS(ld kS_) {
  kS = kS_;
}

void SimpleObject::setReflectance(ld reflectance_) {
  reflectance = reflectance_;
}

void SimpleObject::setTransmittance(ld transmittance_) {
  transmittance = transmittance_;
}

void SimpleObject::setRefractivity(ld refractivity_) {
  assert(refractivity_ > 1 || fabs(double(refractivity_ - 1)) < eps);
  refractivity = refractivity_;
}

vector<ray> SimpleObject::getEmergentRays() {
  vector<ray> emergentRays;
  if (reflectance > eps) {
    gVector dir = -mirror(inl.v, normalVector);
    emergentRays.push_back(ray(Line(intersection + eps * dir, dir), reflectance));
  }
  if (transmittance > eps) {
    if (dot(normalVector, inl.v) < 0) {
      ld i = angle(-normalVector, inl.v);
      ld r = asin(double(sin(i) / refractivity));
      ld len = sin(i - r) / sin(r);
      gVector dir = normalize(inl.v - len * normalVector);
      emergentRays.push_back(ray(Line(intersection + eps * dir, dir), transmittance));
    }
    else {
      ld i = angle(normalVector, inl.v);
      ld r = asin(double(sin(i) * refractivity));
      ld len = sin(r - i) / sin(r);
      gVector dir = normalize(inl.v + len * normalVector);
      emergentRays.push_back(ray(Line(intersection + eps * dir, dir), transmittance));
    }
  }
  return emergentRays;
}

Vec3b SimpleObject::ambientIllumination(Vec3b inten) {
  return kA * inten;
}

Vec3b SimpleObject::localIllumination(Vec3b inten, gVector direction) {

  // judge occlusion
  ld diffuse = -dot(normalVector, direction);
  // gVector outDir = -normalVector * diffuse * 2 - direction;
  gVector outDir = mirror(direction, normalVector);
  ld specular = pow(double(dot(inl.v, outDir)), 3.0);
  return diffuse * kD * inten + specular * kS * inten;
}

void SimpleObject::calc() {
  intersectant = bobj->intersection(inl, intersection, normalVector);
  if (!intersectant) {
    return;
  }

}
