#include <iostream>
#include "scene.h"
#include "mesh.h"
#include "bezier.h"
#include "convex_hull.h"

void onMouse(int event, int x, int y, int flag, void *utsc) {
  if (event == CV_EVENT_LBUTTONUP) {
    std::cout << x << " " << y << std::endl;
  }
}

int main() {
  Scene scene(300, 400, -220, gPoint(200, 200, -50));
  scene.setAmbientLight(new AmbientLight(Vec3b(150, 150, 150)));

  Light *light = new PointLight(gPoint(120, 40, 150), Vec3b(40, 40, 40));
  scene.addLight(light);
  scene.addLight(new PointLight(gPoint(320, 10, 20), Vec3b(200, 200, 200)));
  BasicObject *sphere = new Sphere(gPoint(260, 280, 120), 50);
  SimpleObject *object = new SimpleObject(sphere, Vec3b(40, 200, 10));
  object->setKS(0.9);
  /*
  object->setKD(0, 0, 0);
  object->setKA(0.3, 0.6, 0.3);
  object->setTransmittance(0.8);
  object->setRefractivity(1.2);
   */
  // scene.addObject(object);

  BasicObject *sphere1 = new Sphere(gPoint(420, 240, 410), 130);
  SimpleObject *object1 = new SimpleObject(sphere1, Vec3b(140, 100, 180));
  object1->setKS(0.6);
  // scene.addObject(object1);

  BasicObject *sphere2 = new Sphere(gPoint(380, 240, 210), 40);
  SimpleObject *object2 = new SimpleObject(sphere2, Vec3b(40, 200, 180));
  object2->setKS(0.6);
  object2->setReflectance(0.8);
  // scene.addObject(object2);


  vector<gPoint> vList;
  /*
  vList.push_back(gPoint(170, 480, 200));
  vList.push_back(gPoint(270, 480, 200));
  vList.push_back(gPoint(270, 180, 200));
  vList.push_back(gPoint(170, 180, 200));
  BasicObject *facet = new Facet(vList);
  SimpleObject *object3 = new SimpleObject(facet, Vec3b(70, 20, 40));
  object3->KAMultiple(0.3);
  object3->KDMultiple(0.3);
  object3->setTransmittance(1);
  object3->setRefractivity(1.4);
  scene.addObject(object3);

  vList.clear();
  vList.push_back(gPoint(170, 480, 205));
  vList.push_back(gPoint(170, 180, 205));
  vList.push_back(gPoint(270, 180, 205));
  vList.push_back(gPoint(270, 480, 205));
  facet = new Facet(vList);
  object3 = new SimpleObject(facet, Vec3b(70, 20, 40));
  object3->KAMultiple(0.3);
  object3->KDMultiple(0.3);
  object3->setTransmittance(1);
  object3->setRefractivity(1.4);
  scene.addObject(object3);
   */

  /*
  vList.clear();
  vList.push_back(gPoint(70, 480, 200));
  vList.push_back(gPoint(570, 480, 200));
  vList.push_back(gPoint(570, 480, 255));
  vList.push_back(gPoint(70, 480, 255));
  vList.push_back(gPoint(70, 180, 200));
  vList.push_back(gPoint(570, 180, 200));
  vList.push_back(gPoint(570, 180, 255));
  vList.push_back(gPoint(70, 180, 255));
  BasicObject *cube = new Mesh(vList, vector<orderVector>{
          orderVector{0, 1, 5, 4},
          orderVector{2, 3, 7, 6},
          orderVector{1, 2, 6, 5},
          orderVector{0, 4, 7, 3},
          orderVector{4, 5, 6, 7},
          orderVector{0, 3, 2, 1}
  });
  SimpleObject *object4 = new SimpleObject(cube, Vec3b(80, 100, 150));
  object4->setKS(0.6);
  object4->KAMultiple(0);
  object4->KDMultiple(0);
  // object4->setKA(0.5, 0.5, 0.5);
  // object4->setKD(0.5, 0.5, 0.5);
  // object4->setReflectance(0.3);
  object4->setTransmittance(0.9);
  object4->setRefractivity(1.4);
  object4->setReflectance(0.3);
  scene.addObject(object4);
   */

  vector<gPoint> vertices;
  vertices.push_back(gPoint(250, 480, 200));
  vertices.push_back(gPoint(400, 480, 200));
  vertices.push_back(gPoint(400, 480, 350));
  vertices.push_back(gPoint(250, 480, 350));
  vertices.push_back(gPoint(250, 180, 200));
  vertices.push_back(gPoint(400, 180, 200));
  vertices.push_back(gPoint(400, 180, 350));
  vertices.push_back(gPoint(250, 180, 350));

  // vertices.push_back(gPoint(380, 470, 240));
  // vertices.push_back(gPoint(400, 200, 220));
  // vertices.push_back(gPoint(350, 250, 280));
  // vertices.push_back(gPoint(320, 200, 230));
  // vertices.push_back(gPoint(360, 280, 210));
  Mesh convexHull = getConvexHull(vertices);
  BasicObject *mesh = &convexHull;
  SimpleObject *object5 = new SimpleObject(mesh, Vec3b(230, 130, 150));
  // scene.addObject(object5);

  BasicObject *box = new Box(250, 400, 180, 480, 200, 350);
  SimpleObject *object6 = new SimpleObject(box, Vec3b(200, 140, 180));
  // scene.addObject(object6);

  BasicObject *tube = new Tube(320, 440, 300, 350, 50, 100);
  SimpleObject *object7 = new SimpleObject(tube, Vec3b(100, 50, 180));
  object7->setKS(0.9);
  // scene.addObject(object7);

  // pointMatrix pm{vector<gPoint>{gPoint(100, 100, 100), gPoint(200, 120, 120), gPoint(300, 150, 110)},
  //                vector<gPoint>{gPoint(120, 120, 100), gPoint(180, 150, 150), gPoint(300, 200, 150)},
  //                vector<gPoint>{gPoint(90, 180, 120), gPoint(190, 200, 200), gPoint(290, 290, 230)}};
  // BasicObject *bezier = new BezierSurface(pm);
  // SimpleObject *object8 = new SimpleObject(bezier, Vec3b(50, 60, 70));
  // scene.addObject(object8);

  BasicObject *wall[5];
  wall[0] = new Plane(gPoint(320, 0, 110), gVector(0, 1, 0));
  wall[1] = new Plane(gPoint(0, 240, 110), gVector(1, 0, 0));
  wall[2] = new Plane(gPoint(320, 480, 110) ,gVector(0, -1, 0));
  wall[3] = new Plane(gPoint(640, 240, 110), gVector(-1, 0, 0));
  wall[4] = new Plane(gPoint(320, 240, 520), gVector(0, 0, -1));
  wall[5] = new Plane(gPoint(320, 240, -500), gVector(0, 0, 1));

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
  wa[2] = new SimpleObject(wall[2], new GridTexture(Vec3b(20, 20, 255), Vec3b(255, 255, 255)));
  wa[2]->setKS(0.1);


  /*
  delete wa[4];
  wa[4] = new SimpleObject(wall[4], Vec3b(100, 30, 80));


  wa[3]->setKS(0.2);
  wa[3]->setKD(0, 0, 0);
  wa[3]->setKA(0, 0, 0);
  wa[3]->setReflectance(0.8);
  */


  /*
  wa[1]->setKS(0.2);
  wa[1]->setKD(0, 0, 0);
  wa[1]->setKA(0, 0, 0);
  wa[1]->setReflectance(0.8);
  */

  /*
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
  setMouseCallback("res", onMouse, 0);
  waitKey(0);
  return 0;
}