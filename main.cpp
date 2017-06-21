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
  Scene scene(640, 480, Plane(gPoint(-350, -260, -240), gVector(9.5, 1.5, 1.5)), gVector(1, 0, 0), gPoint(180, 280, 40));
  scene.setFocalPlaneDist(830);
  // scene.superSamplingOn(true);
  scene.setAmbientLight(new AmbientLight(Vec3b(150, 150, 150)));

  Light *light = new PointLight(gPoint(120, 40, 150), Vec3b(40, 40, 40));
  scene.addLight(light);
  scene.addLight(new PointLight(gPoint(320, 10, 20), Vec3b(200, 200, 200)));

  BasicObject *sphere = new Sphere(gPoint(190, 330, 120), 13);
  SimpleObject *object = new SimpleObject(sphere, Vec3b(0, 0, 0));
  object->setKS(0.9);
  object->setKD(0, 0, 0);
  object->setTransmittance(0.8);
  object->setRefractivity(1.2);
  object->setReflectance(0.3);
  // scene.addObject(object);

  BasicObject *sphere1 = new Sphere(gPoint(420, 240, 410), 130);
  SimpleObject *object1 = new SimpleObject(sphere1, Vec3b(140, 100, 180));
  object1->setKS(0.6);
  // scene.addObject(object1);

  BasicObject *sphere2 = new Sphere(gPoint(380, 240, 310), 90);
  SimpleObject *object2 = new SimpleObject(sphere2, new GridTexture(0.08, Vec3b(255, 255, 255), Vec3b(0, 0, 255)));
  // SimpleObject *object2 = new SimpleObject(sphere2, Vec3b(255, 255, 20));
  object2->setKS(0.9);
  // object2->setReflectance(0.8);
  // scene.addObject(object2);


  vector<gPoint> vList;
  vList.push_back(gPoint(160, 469.5, 190));
  vList.push_back(gPoint(422, 469.5, 190));
  vList.push_back(gPoint(422, 469.5, 453));
  vList.push_back(gPoint(160, 469.5, 453));
  BasicObject *facet = new Facet(vList);
  SimpleObject *object3 = new SimpleObject(facet, new GridTexture(6.6));
  object3->setKA(0.2, 0.2, 0.2);
  object3->setKS(0.9);
  object3->setReflectance(0.3);
  scene.addObject(object3);


  // vList.clear();
  // vList.push_back(gPoint(156, 469.7, 186));
  // vList.push_back(gPoint(427, 469.7, 186));
  // vList.push_back(gPoint(427, 469.7, 457));
  // vList.push_back(gPoint(156, 469.7, 457));
  // BasicObject *facet1 = new Facet(vList);
  // SimpleObject *object4 = new SimpleObject(facet1, Vec3b(70, 20, 40));
  // object3->KAMultiple(0.3);
  // object3->KDMultiple(0.3);
  // scene.addObject(object4);

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

  BasicObject *box = new Box(150, 433, 470, 480, 180, 463);
  SimpleObject *object6 = new SimpleObject(box, new ImageTexture("wood.jpeg", 30), true);
  object6->setKA(0.5, 0.5, 0.5);
  scene.addObject(object6);

  BasicObject *tube = new Tube(320, 440, 200, 250, 50, 100);
  SimpleObject *object7 = new SimpleObject(tube, new GridTexture(0.08));
  object7->setKS(0.9);
  // scene.addObject(object7);

  // pointMatrix pm{vector<gPoint>{gPoint(400, 480, 420), gPoint(550, 480, 420)},
  //                vector<gPoint>{gPoint(400, 100, 320), gPoint(550, 100, 320)},
  //                vector<gPoint>{gPoint(400, 480, 220), gPoint(550, 480, 220)}};
  // BasicObject *bezier = new BezierSurface(pm);
  // SimpleObject *object8 = new SimpleObject(bezier, new GridTexture(0.08));
  // scene.addObject(object8);

  // vector<gPoint> pList{gPoint(120, 480, 0), gPoint(100, 440, 0), gPoint(150, 380, 0),
  //                      gPoint(116, 380, 0), gPoint(66, 440, 0), gPoint(84, 480, 0)};
  vector<gPoint> pList{
          // gPoint(0, 220, 0),
          // gPoint(12, 230, 0),
          gPoint(0, 422, 0),
          gPoint(25.3, 422, 0),
          gPoint(3.33, 419, 0),
          gPoint(3.33, 433, 0),
          gPoint(3.33, 441, 0),
          gPoint(3.33, 445, 0),
          gPoint(10, 448, 0),
          gPoint(6, 459, 0),
          gPoint(15.7, 467, 0),
          gPoint(15, 470, 0)
  };
  BasicObject *bezierRotator = new BezierRotator(pList, 307, 305);
  SimpleObject *object9 = new SimpleObject(bezierRotator, new ImageTexture("gold.jpg", 30), true);
  object9->setKA(0.4, 0.4, 0.4);
  // object9->setKD(0, 0, 0);
  object9->setKS(0.97);
  // object9->setTransmittance(0.9);
  // object9->setRefractivity(1.2);
  object9->setReflectance(0.25);
  scene.addObject(object9);
  BasicObject *chessSphere = new Sphere(gPoint(307, 412, 305), 10);
  SimpleObject *object10 = new SimpleObject(chessSphere, new ImageTexture("gold.jpg", 30), true);
  object10->setKS(0.97);
  object10->setKA(0.4, 0.4, 0.4);
  // object10->setTransmittance(0.9);
  // object10->setRefractivity(1.2);
  object10->setReflectance(0.25);
  scene.addObject(object10);

  BasicObject *wall[5];
  wall[0] = new Plane(gPoint(320, 0, 110), gVector(0, 1, 0));
  wall[1] = new Plane(gPoint(0, 240, 110), gVector(1, 0, 0));
  wall[2] = new Plane(gPoint(320, 480, 110) ,gVector(0, -1, 0));
  wall[3] = new Plane(gPoint(640, 240, 110), gVector(-1, 0, 0));
  wall[4] = new Plane(gPoint(320, 240, 520), gVector(0, 0, -1));
  wall[5] = new Plane(gPoint(320, 240, -300), gVector(0, 0, 1));

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
  wa[2] = new SimpleObject(wall[2], new GridTexture(3, Vec3b(30, 30, 255), Vec3b(255, 255, 255)));
  wa[2]->setKS(0.9);

  wa[4]->setKD(0, 0, 0);
  wa[4]->setKA(0, 0, 0);
  wa[4]->setReflectance(0.8);


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
  imwrite("output.jpg", img);
  waitKey(0);
  return 0;
}