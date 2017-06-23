//
// Created by zxcroon on 17-4-20.
//

#include "scene.h"

Scene::Scene(int width_, int height_, Plane screen_, gVector pivot1_, gPoint aperture_, int radius_) :
        width(width_), height(height_), screen(screen_), startR(0), startC(0),
        pivot1(normalize(pivot1_)), pivot2(normalize(cross(screen_.v, pivot1_))),
        aperture(aperture_), ambientLight(new AmbientLight(Vec3b(0, 0, 0))), res(height_, width_, CV_8UC3),
        radius(radius_), focalPlane(screen_.P + fabs(dot(aperture_ - screen_.P, screen_.v)) * 2 * screen_.v, screen_.v), ss(false) {
}

void Scene::load(string imgName, int startR_, int startC_) {
  res = imread(imgName);
  startR = startR_;
  startC = startC_;
}

void Scene::setFocalPlaneDist(ld d) {
  focalPlane = Plane(screen.P + d * screen.v, screen.v);
}

void Scene::superSamplingOn(bool b) {
  ss = b;
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
  vector<Vec3b> colorRec;
  for (int i = 0; i < res.rows; ++i) {
    for (int j = 0; j < res.cols; ++j) {
      if (i < startR || i == startR && j < startC) {
        continue;
      }
      if (!ss) {
        res.at<Vec3b>(i, j) = renderOnce(calcStarting(res.cols - j, res.rows - i));
      }
      else {
        colorRec.clear();
        Vec3i sum(0, 0, 0);
        Vec3b av;
        int ssTimes = 0;
        const ld di[9] = {0, -0.33, -0.33, 0, 0.33, 0.33, 0.33, 0, -0.33};
        const ld dj[9] = {0, 0, 0.33, 0.33, 0.33, 0, -0.33, -0.33, -0.33};
        for (int k = 0; k < 9; ++k) {
          // ld di = rand() % 1000 / 1000.0 - 0.5, dj = rand() % 1000 / 1000.0 - 0.5;
          Vec3b thisRes = renderOnce(calcStarting(res.cols - (j + dj[k]), res.rows - i + di[k]));
          sum += thisRes;
          ++ssTimes;
          av = sum / ssTimes;
        }
        res.at<Vec3b>(i, j) = av;
      }
      printf("%.3lf%%\n", double(i * width + j) / (width * height) * 100);
      if ((i * width + j + 1) % 100 == 0) {
        imwrite("tmp.jpg", res);
        std::ofstream ofs("finished_rec");
        ofs << '(' << i << ',' << j << ')' << std::endl;
        ofs.close();
        std::cout << "tmp saved." << std::endl;
      }
    }
  }
  return res;
}

Vec3b Scene::renderOnce(gPoint startingPoint) {
  Line l(aperture, aperture - startingPoint);
  Vec3i colorSum(0, 0, 0);
  gPoint P;
  gVector v;
  focalPlane.intersection0(l, P, v);

  int rounds = (fabs(radius - 1) < eps ? 1 : radius * 20);
  for (int k = 0; k < rounds; ++k) {
    int rou = rand() % radius;
    ld theta = (ld)(rand() % 180000) / 1000 / 180 * PI;
    ld phi = (ld)(rand() % 360000) / 1000 / 180 * PI;
    gPoint startPoint = aperture + gVector(rou * sin(theta) * cos(phi), rou * sin(theta) * sin(phi), rou * cos(theta));

    Line ray(startPoint, P - startPoint);
    colorSum += rayTracing(ray, 1);
  }
  return colorSum / rounds;
}

gPoint Scene::calcStarting(ld x1, ld x2) {
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