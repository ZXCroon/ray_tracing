//
// Created by zxcroon on 17-4-20.
//

#ifndef RAY_TRACING_OBJECT_H
#define RAY_TRACING_OBJECT_H

#include "basic_object.h"
#include "light.h"
#include "texture.h"

typedef std::pair<Line, ld> ray;

class Object {
public:
  void incidence(Line l);
  bool beIntersectant();
  gPoint getIntersection();
  gVector getNormalVector();
  virtual vector<ray> getEmergentRays() = 0;
  virtual Vec3b ambientIllumination(Vec3b inten) = 0;
  virtual Vec3b localIllumination(Vec3b inten, gVector direction) = 0;

protected:
  Line inl;
  bool intersectant;
  gPoint intersection;
  gVector normalVector;

  virtual void calc() = 0;
};


class SimpleObject : public Object {
public:
  SimpleObject(BasicObject *bobj_, Vec3b color_);
  SimpleObject(BasicObject *bobj_, Texture *texture_, bool textureEnhance_=false);
  ~SimpleObject();

  void setKD(gVector kD_);
  void setKD(ld kDb_, ld kDg_, ld kDr_);
  void setKA(ld kAb_, ld kAg_, ld kAr_);
  void setKS(ld kS_);
  void KDMultiple(ld factor);
  void KAMultiple(ld factor);
  void setReflectance(ld reflectance_);
  void setTransmittance(ld transmittance_);
  void setRefractivity(ld refractivity_);
  void setSpE(ld spE_);
  void fresnelOn(bool b);

  vector<ray> getEmergentRays();
  Vec3b ambientIllumination(Vec3b inten);
  Vec3b localIllumination(Vec3b inten, gVector direction);

private:
  BasicObject *bobj;
  Vec3b color;
  Texture *texture;
  bool haveTexture;
  bool textureEnhance;
  bool fresnel;

  gVector kD, kA, textureCoef;
  ld kS, reflectance, transmittance, refractivity;
  ld spE;

protected:
  void calc();

};

#endif //RAY_TRACING_OBJECT_H
