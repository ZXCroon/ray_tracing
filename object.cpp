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
        texture(NULL), haveTexture(false), textureEnhance(false),
        kD(gVector(color_) / 255),
        kA(gVector(color_) / 255),
        kS(0.4),
        textureCoef(gVector(1, 1, 1)),
        reflectance(0),
        transmittance(0),
        refractivity(1) {
}

SimpleObject::SimpleObject(BasicObject *bobj_, Texture *texture_, bool textureEnhance_) :
        bobj(bobj_), color(Vec3b(0, 0, 0)),
        texture(texture_), haveTexture(true), textureEnhance(textureEnhance_),
        kD(gVector(1, 1, 1)),
        kA(gVector(0.7, 0.7, 0.7)),
        kS(0),
        textureCoef(gVector(1, 1, 1)),
        reflectance(0),
        transmittance(0),
        refractivity(1) {
}

SimpleObject::~SimpleObject() {
  delete bobj;
  bobj = NULL;
  delete texture;
  texture = NULL;
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

void SimpleObject::KDMultiple(ld factor) {
  kD = kD * factor;
}

void SimpleObject::KAMultiple(ld factor) {
  kA = kA * factor;
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
  ld reflectance1 = reflectance;
  if (transmittance > eps) {
    if (dot(normalVector, inl.v) < 0) {
      ld i = angle(-normalVector, inl.v);
      ld sinr = sin(i) / refractivity;
      if (sinr > 1) {
        reflectance1 += transmittance;
        reflectance1 = fmin(1, reflectance1);
      }
      else {
        ld r = asin(double(sinr));
        ld len = sin(i - r) / sin(r);
        gVector dir = normalize(inl.v - len * normalVector);
        emergentRays.push_back(ray(Line(intersection + eps * dir, dir), transmittance));
      }
    }
    else {
      ld i = angle(normalVector, inl.v);
      ld r = asin(double(sin(i) * refractivity));
      ld len = sin(r - i) / sin(r);
      gVector dir = normalize(inl.v + len * normalVector);
      emergentRays.push_back(ray(Line(intersection + eps * dir, dir), transmittance));
    }
    if (transmittance > 1) {
      std::cout << "A" << std::endl;
    }
  }
  if (reflectance > 1) {
    std::cout << reflectance << std::endl;
  }
  if (reflectance1 > eps) {
    gVector dir = -mirror(inl.v, normalVector);
    emergentRays.push_back(ray(Line(intersection + eps * dir, dir), reflectance1));
  }
  return emergentRays;
}

Vec3b SimpleObject::ambientIllumination(Vec3b inten) {
  return kA * inten;
}

Vec3b SimpleObject::localIllumination(Vec3b inten, gVector direction) {

  // judge occlusion
  ld diffuse = fabs(dot(normalVector, direction));
  // gVector outDir = -normalVector * diffuse * 2 - direction;
  gVector outDir = mirror(direction, normalVector);
  ld specular = pow(double(dot(inl.v, outDir)), 3.0);
  if (haveTexture) {
    return diffuse * kD * textureCoef * (textureEnhance ? Vec3b(255, 255, 255) : inten) +
            specular * kS * textureCoef * Vec3b(255, 255, 255);
  }
  return diffuse * kD * inten + specular * kS * inten;
}

void SimpleObject::calc() {
  UvParam uv;
  intersectant = bobj->intersection(inl, intersection, normalVector, uv);
  if (!intersectant) {
    return;
  }
  if (haveTexture) {
    Vec3b color = texture->getColor(uv.first, uv.second);
    textureCoef = gVector(color[0] / 255.0, color[1] / 255.0, color[2] / 255.0);
  }
}
