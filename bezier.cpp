//
// Created by zxcroon on 17-4-29.
//

#include "bezier.h"

BezierRotator::BezierRotator(vector<gPoint> pList_, ld xPivot_, ld zPivot_) :
        pList(pList_), xPivot(xPivot_), zPivot(zPivot_) {}

bool BezierRotator::intersection(Line l, gPoint &I, gVector &n) {

}

const ld BezierSurface::TINY = 0.0001;

BezierSurface::BezierSurface(pointMatrix controllP_) :
        controllP(controllP_), M(controllP_.size() - 1), N(controllP_[0].size() - 1) {
  bounding = genBounding(0, 1, 0, 1);
}

bool BezierSurface::intersection(Line l, gPoint &I, gVector &n) {
  vector<ItsInfo> itsInfos = bounding->intersection(l);
  Mat M(3, 3, CV_64FC1);
  Mat F(3, 1, CV_64FC1);
  Mat X(3, 1, CV_64FC1);
  Mat X_(3, 1, CV_64FC1);
  for (vector<ItsInfo>::iterator it = itsInfos.begin(); it != itsInfos.end(); ++it) {
    ItsInfo itsInfo = *it;
    ld umin = itsInfo.info[0], umax = itsInfo.info[1], vmin = itsInfo.info[2], vmax = itsInfo.info[3];
    ld u = (umin + umax) / 2, v = (vmin + vmax) / 2, t = (itsInfo.P.x - l.P.x) / l.v.x;

    for (int k = 0; k < 10; ++k) {
      gPoint f = value(u, v) - (l.P + t * l.v);
      if (norm(f) < 0.01) {
        break;
      }

      gPoint du = derivativeU(u, v);
      gPoint dv = derivativeV(u, v);

      M.at<double>(0, 0) = double(du[0]);
      M.at<double>(0, 1) = double(dv[0]);
      M.at<double>(0, 2) = double(-l.v[0]);
      M.at<double>(1, 0) = double(du[1]);
      M.at<double>(1, 1) = double(dv[1]);
      M.at<double>(1, 2) = double(-l.v[1]);
      M.at<double>(2, 0) = double(du[2]);
      M.at<double>(2, 1) = double(dv[2]);
      M.at<double>(2, 2) = double(-l.v[2]);

      F.at<double>(0, 0) = double(f[0]);
      F.at<double>(1, 0) = double(f[1]);
      F.at<double>(2, 0) = double(f[2]);

      X.at<double>(0, 0) = double(u);
      X.at<double>(1, 0) = double(v);
      X.at<double>(2, 0) = double(t);

      X_ = X - M.inv() * F;
      u = X_.at<double>(0, 0);
      v = X_.at<double>(1, 0);
      t = X_.at<double>(2, 0);
    }

    gPoint f = value(u, v) - (l.P + t * l.v);
    if (norm(f) < 0.01) {
      I = l.P + t * l.v;
      n = normalize(cross(derivativeU(u, v), derivativeV(u, v)));
      return true;
    }
  }

  return false;
}

Bounding *BezierSurface::genBounding(ld umin, ld umax, ld vmin, ld vmax) {
  if (umax - umin < TINY && vmax - vmin < TINY) {
    gPoint P1 = value(umin, vmin);
    gPoint P2 = value(umin, vmax);
    gPoint P3 = value(umax, vmin);
    gPoint P4 = value(umax, vmax);
    Closed *boundingBox = new Box(fmin(fmin(P1.x, P2.x), fmin(P3.x, P4.x)),
                                  fmax(fmax(P1.x, P2.x), fmax(P3.x, P4.x)),
                                  fmin(fmin(P1.y, P2.y), fmin(P3.y, P4.y)),
                                  fmax(fmax(P1.y, P2.y), fmax(P3.y, P4.y)),
                                  fmin(fmin(P1.z, P2.z), fmin(P3.z, P4.z)),
                                  fmax(fmax(P1.z, P2.z), fmax(P1.z, P2.z)));
    return new Bounding(boundingBox, vector<ld>{umin, umax, vmin, vmax});
  }
  ld umid = (umin + umax) / 2, vmid = (vmin + vmax) / 2;
  Bounding *bounding1 = genBounding(umin, umid, vmin, vmid);
  Bounding *bounding2 = genBounding(umin, umid, vmid, vmax);
  Bounding *bounding3 = genBounding(umid, umax, vmin, vmid);
  Bounding *bounding4 = genBounding(umid, umax, vmid, vmax);

  Box *box1 = static_cast<Box*>(bounding1->getVolume());
  Box *box2 = static_cast<Box*>(bounding2->getVolume());
  Box *box3 = static_cast<Box*>(bounding3->getVolume());
  Box *box4 = static_cast<Box*>(bounding4->getVolume());

  ld xmin = fmin(fmin(box1->xmin, box2->xmin), fmin(box3->xmin, box4->xmin));
  ld xmax = fmax(fmax(box1->xmax, box2->xmax), fmax(box3->xmax, box4->xmax));
  ld ymin = fmin(fmin(box1->ymin, box2->ymin), fmin(box3->ymin, box4->ymin));
  ld ymax = fmax(fmax(box1->ymax, box2->ymax), fmax(box3->ymax, box4->ymax));
  ld zmin = fmin(fmin(box1->zmin, box2->zmin), fmin(box3->zmin, box4->zmin));
  ld zmax = fmax(fmax(box1->zmax, box2->zmax), fmax(box3->zmax, box4->zmax));

  Closed *boundingBox = new Box(xmin, xmax, ymin, ymax, zmin, zmax);
  Bounding *thisBounding = new Bounding(boundingBox);
  thisBounding->addChild(bounding1);
  thisBounding->addChild(bounding2);
  thisBounding->addChild(bounding3);
  thisBounding->addChild(bounding4);
  return thisBounding;
}

gPoint BezierSurface::value(ld u, ld v) {
  gPoint res(0, 0, 0);
  for (int i = 0; i <= M; ++i) {
    for (int j = 0; j <= N; ++j) {
      res = res + Bernstein(i, M, u) * Bernstein(j, N, v) * controllP[i][j];
    }
  }
  return res;
}

gPoint BezierSurface::derivativeU(ld u, ld v) {
  gPoint res(0, 0, 0);
  gPoint res1(0, 0, 0);
  for (int i = 0; i <= M; ++i) {
    for (int j = 0; j <= N; ++j) {
      res = res + derivativeOfBernstein(i, M, u) * Bernstein(j, N, v) * controllP[i][j];
      res1 = res1 + Bernstein(i, M, u) * Bernstein(j, N, v) * controllP[i][j];
    }
  }
  return res;
}

gPoint BezierSurface::derivativeV(ld u, ld v) {
  gPoint res(0, 0, 0);
  for (int i = 0; i <= M; ++i) {
    for (int j = 0; j <= N; ++j) {
      res = res + Bernstein(i, M, u) * derivativeOfBernstein(j, N, v) * controllP[i][j];
    }
  }
  return res;
}