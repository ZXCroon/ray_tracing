#include "light.h"

PointLight::PointLight(gPoint position_, Vec3b intensity_) : position(position_), intensity(intensity_) {}

gPoint PointLight::getPosition() {
  return position;
}

void PointLight::getLight(gPoint point, Vec3b &inten, gVector &direction) {
  inten = intensity;
  direction = normalize(point - position);
}

AmbientLight::AmbientLight(Vec3b intensity_) : intensity(intensity_) {}

Vec3b AmbientLight::getIntensity() {
  return intensity;
}
