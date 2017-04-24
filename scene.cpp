//
// Created by zxcroon on 17-4-20.
//

#include "scene.h"

Scene::Scene(int height_, int width_, gPoint viewpoint_) :
        height(height_), width(width_), viewpoint(viewpoint_), ambientLight(new AmbientLight(Vec3b(0, 0, 0))) {}

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
      res.at<Vec3b>(i, j) = rayTracing(Line(viewpoint, gVector(j, i, 0) - viewpoint), 1);
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
    if (!occluded) {
      color += firstObject->localIllumination(inten, direction);
    }
    else {
      std::cout << firstObject->getIntersection() << std::endl;
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