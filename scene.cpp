//
// Created by zxcroon on 17-4-20.
//

#include "scene.h"

Scene::Scene(int height_, int width_, int zScreen_, gPoint aperture_) :
        height(height_), width(width_), zScreen(zScreen_), aperture(aperture_), ambientLight(new AmbientLight(Vec3b(0, 0, 0))),
        radius(1) {}

Scene::Scene(int height_, int width_, int zScreen_, gPoint aperture_, int radius_) :
        height(height_), width(width_), zScreen(zScreen_), aperture(aperture_), ambientLight(new AmbientLight(Vec3b(0, 0, 0))),
        radius(radius_) {}

void Scene::addObject(Object *object_) {
  objects.push_back(object_);
}

void Scene::setAmbientLight(AmbientLight *ambientLight_) {
  ambientLight = ambientLight_;
}

void Scene::addLight(Light *light_) {
  lights.push_back(light_);
}

Mat Scene::render() {
  Mat res(height, width, CV_8UC3);
  for (int i = 0; i < res.rows; ++i) {
    for (int j = 0; j < res.cols; ++j) {
      Line l(aperture, aperture - gPoint(res.cols - j, res.rows - i, zScreen));
      Vec3i colorSum(0, 0, 0);
      Plane *focalPlane = new Plane(gPoint(0, 0, -zScreen), gVector(0, 0, -1));
      gPoint P;
      gVector v;
      focalPlane->intersection0(l, P, v);

      if (i == 89 && j == 301) {
        std::cout << "ze" << std::endl;
      }

      for (int k = 0; k < radius * 10; ++k) {
        int rou = rand() % radius;
        ld theta = (ld)(rand() % 360000) / 1000 / 180 * 3.1415926;
        gPoint startPoint = aperture + gVector(rou * cos(theta), rou * sin(theta), 0);

        Line ray(startPoint, P - startPoint);
        colorSum += rayTracing(ray, 1);
      }

      res.at<Vec3b>(i, j) = colorSum / (radius * 10);
      printf("%.1lf%%\n", double(i * width + j) / (width * height) * 100);
    }
  }
  return res;
}

Vec3b Scene::rayTracing(Line l, ld decay) {
  Vec3b color(0, 0, 0);
  if (decay < 0.2) {
    return color;
  }

  for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
    (*it)->incidence(l);
  }

  Object *firstObject = NULL;
  for (vector<Object *>::iterator it = objects.begin(); it != objects.end(); ++it) {
    if (!(*it)->beIntersectant()) {
      continue;
    }
    if (!firstObject || norm((*it)->getIntersection() - l.P) < norm(firstObject->getIntersection() - l.P)) {
      firstObject = *it;
    }
  }

  if (!firstObject) {
    return color;
  }

  color += firstObject->ambientIllumination(ambientLight->getIntensity());
  for (vector<Light *>::iterator it = lights.begin(); it != lights.end(); ++it) {
    Vec3b inten;
    gVector direction(0, 0, 0);
    (*it)->getLight(firstObject->getIntersection(), inten, direction);

    Line shadowRay(firstObject->getIntersection() - eps * direction, -direction);
    bool occluded = false;
    for (vector<Object *>::iterator it1 = objects.begin(); it1 != objects.end(); ++it1) {
      if ((*it1) == firstObject) {
        continue;
      }
      (*it1)->incidence(shadowRay);
      if ((*it1)->beIntersectant() &&
              distance((*it1)->getIntersection(), firstObject->getIntersection()) <
                      distance((*it)->getPosition(), firstObject->getIntersection())) {
        occluded = true;
        break;
      }
    }
    ///
    // occluded = false;
    ///

    if (!occluded) {
      color += firstObject->localIllumination(inten, direction);
    }
  }

  vector<ray> rays = firstObject->getEmergentRays();
  for (vector<ray>::iterator it = rays.begin(); it != rays.end(); ++it) {
    if (it->second > eps) {
      color += rayTracing(it->first, decay * it->second);
    }
  }
  return decay * color;
}