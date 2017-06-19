//
// Created by zxcroon on 17-5-2.
//

#ifndef RAY_TRACING_BOUNDING_H
#define RAY_TRACING_BOUNDING_H

#include "basic_object.h"

class Bounding;

class ItsInfo {
public:
  gPoint P;
  Line l;
  Bounding *bounding;
  vector<ld> info;
  ItsInfo(gPoint P_, Line l_, Bounding *bounding_, vector<ld> info_);
};

bool operator<(ItsInfo itsInfo1, ItsInfo itsInfo2);

class Bounding {
public:
  Bounding(Closed *volume_);
  Bounding(Closed *volume_, vector<ld> info_);
  Closed *getVolume();

  void addChild(Bounding *newChild);
  vector<ItsInfo> intersection(Line l);
private:
  Closed *volume;
  vector<ld> info;
  vector<Bounding*> children;
  vector<ItsInfo> intersectionWithoutSorting(Line l);
};


#endif //RAY_TRACING_BOUNDING_H
