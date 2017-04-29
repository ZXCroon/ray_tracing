//
// Created by zxcroon on 17-4-29.
//

#ifndef RAY_TRACING_MESH_H
#define RAY_TRACING_MESH_H

#include "basic_object.h"

class Mesh : public BasicObject {
public:
  Mesh(vector<gPoint> vList_, vector<orderVector> fList_);
  bool intersection(Line l, gPoint &I, gVector &n);

private:
  vector<gPoint> vList;
  vector <orderVector> fList;
};

#endif //RAY_TRACING_MESH_H
