#include <iostream>
#include "scene.h"

int main() {
  Scene scene(480, 640, -450, gPoint(320, 280, -200));
  scene.setAmbientLight(new AmbientLight(Vec3b(150, 150, 150)));

  Light *light = new PointLight(gPoint(120, 40, 150), Vec3b(40, 40, 40));
  scene.addLight(light);
  scene.addLight(new PointLight(gPoint(320, 10, 20), Vec3b(200, 200, 200)));
  BasicObject *sphere = new Sphere(gPoint(340, 380, 100), 100);
  SimpleObject *object = new SimpleObject(sphere, Vec3b(40, 200, 10));
  // object->setKS(0.9);
  object->setKS(0.6);
  object->setKD(0, 0, 0);
  object->setKA(0.3, 0.6, 0.3);
  object->setTransmittance(0.8);
  object->setRefractivity(1.1);
  scene.addObject(object);

  BasicObject *sphere1 = new Sphere(gPoint(320, 340, 210), 80);
  SimpleObject *object1 = new SimpleObject(sphere1, Vec3b(140, 100, 180));
  object1->setKS(0.6);
  scene.addObject(object1);

  BasicObject *wall[5];
  wall[0] = new Plane(gPoint(320, 0, 110), gVector(0, 1, 0));
  wall[1] = new Plane(gPoint(0, 240, 110), gVector(1, 0, 0));
  wall[2] = new Plane(gPoint(320, 480, 110) ,gVector(0, -1, 0));
  wall[3] = new Plane(gPoint(640, 240, 110), gVector(-1, 0, 0));
  wall[4] = new Plane(gPoint(320, 240, 420), gVector(0, 0, -1));
  wall[5] = new Plane(gPoint(320, 240, -600), gVector(0, 0, 1));

  SimpleObject *wa[6];
  for (int i = 0; i < 6; ++i) {
    wa[i] = new SimpleObject(wall[i], Vec3b(180, 200, 30));
    wa[i]->setKS(0.1);
    // scene.addObject(wa[i]);

    /*
    wa[i]->setKS(0.2);
    wa[i]->setKD(0, 0, 0);
    wa[i]->setKA(0, 0, 0);
    wa[i]->setReflectance(0.93);
     */

  }

  /*
  delete wa[4];
  wa[4] = new SimpleObject(wall[4], Vec3b(100, 30, 80));

  wa[3]->setKS(0.2);
  wa[3]->setKD(0, 0, 0);
  wa[3]->setKA(0, 0, 0);
  wa[3]->setReflectance(0.8);
   */

  /*
  wa[4]->setKS(0.2);
  wa[4]->setKD(0, 0, 0);
  wa[4]->setKA(0, 0, 0);
  wa[4]->setReflectance(0.5);

  wa[5]->setKS(0.2);
  wa[5]->setKD(0, 0, 0);
  wa[5]->setKA(0, 0, 0);
  wa[5]->setReflectance(0.5);
   */

  scene.addObject(wa[0]);
  scene.addObject(wa[1]);
  scene.addObject(wa[2]);
  scene.addObject(wa[3]);
  scene.addObject(wa[4]);
  scene.addObject(wa[5]);

  Mat img = scene.render();
  namedWindow("res");
  imshow("res", img);
  waitKey(0);
  return 0;
}