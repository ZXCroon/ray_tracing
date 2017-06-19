//
// Created by zxcroon on 17-5-2.
//

#include "bounding.h"

ItsInfo::ItsInfo(gPoint P_, Line l_, Bounding *bounding_, vector<ld> info_) : P(P_), l(l_), bounding(bounding_), info(info_) {}

bool operator<(ItsInfo itsInfo1, ItsInfo itsInfo2) {
  return distance(itsInfo1.P, itsInfo1.l.P) < distance(itsInfo2.P, itsInfo2.l.P);
}

Bounding::Bounding(Closed *volume_) : volume(volume_) {}

Bounding::Bounding(Closed *volume_, vector<ld> info_) : volume(volume_), info(info_) {}

Closed *Bounding::getVolume() {
  return volume;
}

void Bounding::addChild(Bounding *newChild) {
  children.push_back(newChild);
}

vector<ItsInfo> Bounding::intersection(Line l) {
  vector<ItsInfo> res = intersectionWithoutSorting(l);
  sort(res.begin(), res.end());
  return res;
}

vector<ItsInfo> Bounding::intersectionWithoutSorting(Line l) {
  vector<ItsInfo> res;
  gPoint I;
  gVector n;
  if (!volume->intersection0(l, I, n)) {
    return res;
  }
  if (children.empty()) {
    res.push_back(ItsInfo(I, l, this, info));
    return res;
  }
  for (vector<Bounding*>::iterator it = children.begin(); it != children.end(); ++it) {
    vector<ItsInfo> res0 = (*it)->intersection(l);
    res.insert(res.end(), res0.begin(), res0.end());
  }
  return res;
}
