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

int main(int argc, char **argv) {
  //
  // Basic settings of scene
  //
  Scene scene(640, 480, Plane(gPoint(-350, -260, -240), gVector(9.5, 1.5, 1.5)), gVector(1, 0, 0), gPoint(180, 280, 40), 7);
  scene.setFocalPlaneDist(830);
  scene.superSamplingOn(true);
  scene.setAmbientLight(new AmbientLight(Vec3b(150, 150, 150)));
  if (argc >= 4) {
    scene.load(argv[1], atoi(argv[2]), atoi(argv[3]));
  }


  //
  // Lights
  //
  scene.addLight(new PointLight(gPoint(120, 40, 150), Vec3b(40, 40, 40)));
  scene.addLight(new PointLight(gPoint(320, 10, 20), Vec3b(200, 200, 200)));


  //
  // Sphere
  //
  BasicObject *sphere = new Sphere(gPoint(190, 330, 120), 13);
  SimpleObject *sphereObject = new SimpleObject(sphere, Vec3b(0, 0, 0));
  sphereObject->setKS(0.9);
  sphereObject->setTransmittance(0.8);
  sphereObject->setRefractivity(1.2);
  sphereObject->setReflectance(0.3);
  scene.addObject(sphereObject);


  //
  // Chessboard grids
  //
  vector<gPoint> vList;
  vList.push_back(gPoint(160, 469.7, 190));
  vList.push_back(gPoint(422, 469.7, 190));
  vList.push_back(gPoint(422, 469.7, 453));
  vList.push_back(gPoint(160, 469.7, 453));
  BasicObject *facet = new Facet(vList);
  SimpleObject *chessboard = new SimpleObject(facet, new GridTexture(6.6));
  chessboard->setKA(0.2, 0.2, 0.2);
  chessboard->setKS(0.9);
  chessboard->setReflectance(0.3);
  scene.addObject(chessboard);


  //
  // Chessboard wood
  //
  BasicObject *box = new Box(150, 433, 470, 480, 180, 463);
  SimpleObject *wood = new SimpleObject(box, new ImageTexture("wood.jpeg", 30), true);
  wood->setKA(0.5, 0.5, 0.5);
  scene.addObject(wood);


  //
  // flag
  //
  BasicObject *tube = new Tube(415, 465, 340, 240, 1, 2);
  SimpleObject *flagStick = new SimpleObject(tube, Vec3b(0, 0, 0));
  flagStick->setKS(0.8);
  flagStick->setTransmittance(0.9);
  flagStick->setRefractivity(1.2);
  flagStick->setReflectance(0.23);
  scene.addObject(flagStick);

  BasicObject *sphere1 = new Sphere(gPoint(340, 413, 240), 3);
  SimpleObject *flagSphere = new SimpleObject(sphere1, Vec3b(0, 0, 0));
  scene.addObject(flagSphere);
  flagSphere->setKS(0.8);
  flagSphere->setTransmittance(0.9);
  flagSphere->setRefractivity(1.2);
  flagSphere->setReflectance(0.23);

  pointMatrix pm{vector<gPoint>{gPoint(336.5, 415, 236.5), gPoint(336.5, 443, 236.5)},
                 vector<gPoint>{gPoint(357, 415, 245), gPoint(357, 443, 245)},
                 vector<gPoint>{gPoint(360, 415, 223), gPoint(360, 443, 223)},
                 vector<gPoint>{gPoint{365, 415, 225}, gPoint(365, 443, 225)}};
  BasicObject *bezier = new BezierSurface(pm);
  SimpleObject *flag = new SimpleObject(bezier, new ImageTexture("your_name.jpg", 1), true);
  flag->setKA(0.4, 0.4, 0.4);
  scene.addObject(flag);

  vector<gPoint> pList{
          gPoint(3, 465, 0),
          gPoint(10, 469.9, 0),
          gPoint(0, 469.9, 0)
  };
  BasicObject *bR = new BezierRotator(pList, 340, 240);
  SimpleObject *flagBase = new SimpleObject(bR, Vec3b(0, 0, 0));
  flagBase->setKS(0.8);
  flagBase->setTransmittance(0.9);
  flagBase->setRefractivity(1.2);
  flagBase->setReflectance(0.23);
  scene.addObject(flagBase);


  //
  // Chess piece
  //
  vector<gPoint> pList1{
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
  BasicObject *bR1 = new BezierRotator(pList1, 307, 305);
  SimpleObject *chessBase = new SimpleObject(bR1, new ImageTexture("gold.jpg", 30), true);
  chessBase->setKA(0.4, 0.4, 0.4);
  chessBase->setKS(0.97);
  chessBase->setReflectance(0.25);
  scene.addObject(chessBase);
  BasicObject *sphere2 = new Sphere(gPoint(307, 412, 305), 10);
  SimpleObject *chessSphere = new SimpleObject(sphere2, new ImageTexture("gold.jpg", 30), true);
  chessSphere->setKS(0.97);
  chessSphere->setKA(0.4, 0.4, 0.4);
  chessSphere->setReflectance(0.25);
  scene.addObject(chessSphere);


  //
  // Cup
  //
  BasicObject *bR2 = new Tube(430, 480, 483, 400, 24, 25);
  SimpleObject *cupBody = new SimpleObject(bR2, new ImageTexture("porcelain.jpg", 30));
  cupBody->setKA(0.2, 0.2, 0.2);
  scene.addObject(cupBody);

  pointMatrix pm1{vector<gPoint>{gPoint(483, 442, 378), gPoint(481, 440, 378),
                                 gPoint(483, 438, 378), gPoint(485, 440, 378),
                                 gPoint(483, 442, 378)},
                  vector<gPoint>{gPoint(483, 453, 345), gPoint(481, 453, 343),
                                 gPoint(485, 453, 341), gPoint(485, 453, 343),
                                 gPoint(483, 453, 345)},
                  vector<gPoint>{gPoint(483, 470, 378), gPoint(481, 472, 378),
                                 gPoint(482, 474, 378), gPoint(485, 472, 378),
                                 gPoint(483, 470, 378)}};
  BasicObject *bezier1 = new BezierSurface(pm1);
  SimpleObject *cupHandle = new SimpleObject(bezier1, Vec3b(115, 115, 115));
  scene.addObject(cupHandle);


  //
  // Planes
  //
  BasicObject *wall[5];
  wall[0] = new Plane(gPoint(320, 0, 110), gVector(0, 1, 0));
  wall[1] = new Plane(gPoint(0, 240, 110), gVector(1, 0, 0));
  wall[2] = new Plane(gPoint(320, 480, 110) ,gVector(0, -1, 0));
  wall[3] = new Plane(gPoint(560, 240, 110), gVector(-1, 0, 0));
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
  wa[0] = new SimpleObject(wall[0], Vec3b(10, 10, 240));
  wa[1] = new SimpleObject(wall[1], Vec3b(10, 230, 50));
  wa[2] = new SimpleObject(wall[2], new GridTexture(3, Vec3b(30, 30, 255), Vec3b(255, 255, 255)));
  wa[2]->setKS(0.9);

  wa[4]->setKD(0, 0, 0);
  wa[4]->setKA(0, 0, 0);
  wa[4]->setReflectance(0.8);


  scene.addObject(wa[0]);
  // scene.addObject(wa[1]);
  scene.addObject(wa[2]);
  scene.addObject(wa[3]);
  scene.addObject(wa[4]);
  // scene.addObject(wa[5]);

  // static_cast<BezierSurface *>(bezier)->writeToObj("flag.obj");
  // static_cast<BezierSurface *>(bezier1)->writeToObj("cup_handle.obj");
  // static_cast<BezierRotator *>(bR)->writeToObj("flag_base.obj");
  // static_cast<BezierRotator *>(bR1)->writeToObj("chess_base.obj");
  Mat img = scene.render();
  namedWindow("res");
  imshow("res", img);
  setMouseCallback("res", onMouse, 0);
  imwrite("output.jpg", img);
  waitKey(0);
  return 0;
}