//
// Created by zxcroon on 17-4-28.
//

#include "mesh.h"

Mesh::Mesh(vector<gPoint> vList_, vector<orderVector> fList_) : vList(vList_), fList(fList_) {}

bool Mesh::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  bool its = false;
  for (vector<orderVector>::iterator it = fList.begin(); it != fList.end(); ++it) {
    vector<gPoint> vList1;
    for (vector<int>::iterator it1 = (*it).begin(); it1 != (*it).end(); ++it1) {
      vList1.push_back(vList[(*it1)]);
    }
    Facet facet(vList1);
    gPoint I1;
    gVector n1;
    if (facet.intersection(l, I1, n1, uv)) {
      if (!its || distance(I1, l.P) < distance(I, l.P)) {
        I = I1;
        n = n1;
      }
      its = true;
    }
  }
  return its;
}
