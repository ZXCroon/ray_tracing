//
// Created by zxcroon on 17-4-20.
//

#include "scene.h"

Scene::Scene(int width_, int height_, Plane screen_, gVector pivot1_, gPoint aperture_, int radius_) :
        width(width_), height(height_), screen(screen_),
        pivot1(normalize(pivot1_)), pivot2(normalize(cross(screen_.v, pivot1_))),
        aperture(aperture_), ambientLight(new AmbientLight(Vec3b(0, 0, 0))),
        radius(radius_), focalPlane(screen_.P + fabs(dot(aperture_ - screen_.P, screen_.v)) * 2 * screen_.v, screen_.v) {
  gPoint P0(307, 430, 305);
  ld tmp = dot(P0 - screen.P, screen.v);
  std::cout << tmp << std::endl;
}

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
      Line l(aperture, aperture - calcStarting(res.cols - j, res.rows - i));
      Vec3i colorSum(0, 0, 0);
      gPoint P;
      gVector v;
      focalPlane.intersection0(l, P, v);

      if (j == 276 && i == 272) {
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

gPoint Scene::calcStarting(int x1, int x2) {
  return screen.P + x1 * pivot1 + x2 * pivot2;
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