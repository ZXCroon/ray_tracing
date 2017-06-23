#include "light.h"

PointLight::PointLight(gPoint position_, Vec3b intensity_) : position(position_), intensity(intensity_) {}

gPoint PointLight::getPosition() {
  return position;
}

void PointLight::getLight(gPoint point, Vec3b &inten, gVector &direction, int radius) {
  inten = intensity;
  ld r = rand() % (radius * 100) / 100.0;
  ld theta = rand() % 18000 / 100.0 * PI / 180;
  ld phi = rand() % 36000 / 100.0 * PI / 180;
  direction = normalize(point + gPoint(r * sin(theta) * cos(phi), r * sin(theta) * sin(phi), r * cos(theta)) - position);
}

AmbientLight::AmbientLight(Vec3b intensity_) : intensity(intensity_) {}

Vec3b AmbientLight::getIntensity() {
  return intensity;
}
