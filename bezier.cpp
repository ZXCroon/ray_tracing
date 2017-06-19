//
// Created by zxcroon on 17-4-29.
//

#include "bezier.h"

const ld BezierRotator::TINY = 0.01;

BezierRotator::BezierRotator(vector<gPoint> pList_, ld xPivot_, ld zPivot_) :
        pList(pList_), xPivot(xPivot_), zPivot(zPivot_), N(pList_.size() - 1) {
  for (vector<gPoint>::iterator it = pList.begin(); it != pList.end(); ++it) {
    assert(it->x > 0 && it->z == 0);
  }
  bounding = genBounding(0, 1);
}

bool BezierRotator::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  vector<ItsInfo> itsInfos = bounding->intersection(l);
  Mat M(3, 3, CV_64FC1);
  Mat F(3, 1, CV_64FC1);
  Mat X(3, 1, CV_64FC1);
  Mat X_(3, 1, CV_64FC1);
  for (vector<ItsInfo>::iterator it = itsInfos.begin(); it != itsInfos.end(); ++it) {
    ItsInfo itsInfo = *it;
    ld tmin = itsInfo.info[0], tmax = itsInfo.info[1];
    ld tb = (tmin + tmax) / 2, tl = (itsInfo.P.z - l.P.z) / l.v.z;
    ld theta = atan2(itsInfo.P.z - zPivot, itsInfo.P.x - xPivot);
    if (itsInfo.P.z < zPivot) {
      theta += 2 * PI;
    }

    for (int k = 0; k < 10; ++k) {
      gPoint rValue = relativeValue(tb);
      gPoint f = value(tb, theta) - (l.P + tl * l.v);
      if (norm(f) < 0.01) {
        break;
      }

      gPoint dtb, dtheta;
      getDerivative(tb, theta, dtb, dtheta);

      M.at<double>(0, 0) = double(dtb[0]);
      M.at<double>(0, 1) = double(dtheta[0]);
      M.at<double>(0, 2) = double(-l.v[0]);
      M.at<double>(1, 0) = double(dtb[1]);
      M.at<double>(1, 1) = double(dtheta[1]);
      M.at<double>(1, 2) = double(-l.v[1]);
      M.at<double>(2, 0) = double(dtb[2]);
      M.at<double>(2, 1) = double(dtheta[2]);
      M.at<double>(2, 2) = double(-l.v[2]);

      F.at<double>(0, 0) = double(f[0]);
      F.at<double>(1, 0) = double(f[1]);
      F.at<double>(2, 0) = double(f[2]);

      X.at<double>(0, 0) = double(tb);
      X.at<double>(1, 0) = double(theta);
      X.at<double>(2, 0) = double(tl);

      X_ = X - M.inv() * F;
      tb = X_.at<double>(0, 0);
      theta = X_.at<double>(1, 0);
      tl = X_.at<double>(2, 0);
    }

    gPoint f = value(tb, theta) - (l.P + tl * l.v);
    if (norm(f) < 0.01) {
      I = l.P + tl * l.v;
      gPoint dtb, dtheta;
      getDerivative(tb, theta, dtb, dtheta);
      n = normalize(cross(dtb, dtheta));
      uv = UvParam(tb, theta / (2 * PI));
      return true;
    }
  }

  return false;

}

Bounding *BezierRotator::genBounding(ld tmin, ld tmax) {
  if (tmax - tmin < TINY) {
    gPoint P1 = relativeValue(tmin);
    gPoint P2 = relativeValue(tmax);
    Closed *boundingTube = new Tube(fmin(P1.y, P2.y), fmax(P1.y, P2.y),
                                    xPivot, zPivot,
                                    fmin(P1.x, P2.x), fmax(P1.x, P2.x));
    return new Bounding(boundingTube, vector<ld>{tmin, tmax});
  }
  ld tmid = (tmin + tmax) / 2;
  Bounding *bounding1 = genBounding(tmin, tmid);
  Bounding *bounding2 = genBounding(tmid, tmax);

  Tube *tube1 = static_cast<Tube*>(bounding1->getVolume());
  Tube *tube2 = static_cast<Tube*>(bounding2->getVolume());

  ld ymin = fmin(tube1->ymin, tube2->ymin);
  ld ymax = fmax(tube1->ymax, tube2->ymax);
  ld rmin = fmin(tube1->rmin, tube2->rmin);
  ld rmax = fmax(tube1->rmax, tube2->rmax);

  Closed *boundingTube = new Tube(ymin, ymax, xPivot, zPivot, rmin, rmax);
  Bounding *thisBounding = new Bounding(boundingTube);
  thisBounding->addChild(bounding1);
  thisBounding->addChild(bounding2);
  return thisBounding;
}

gPoint BezierRotator::relativeValue(ld t) {
  gPoint res(0, 0, 0);
  for (int i = 0; i <= N; ++i) {
    res = res + Bernstein(i, N, t) * pList[i];
  }
  return res;
}

gPoint BezierRotator::value(ld t, ld theta) {
  gPoint res0 = relativeValue(t);
  return gPoint(xPivot + res0.x * cos(theta), res0.y, zPivot + res0.x * sin(theta));
}

void BezierRotator::getDerivative(ld t, ld theta, gPoint &dT, gPoint &dTheta) {
  gPoint rValue = relativeValue(t);
  gPoint dCdt(0, 0, 0);
  for (int i = 0; i <= N; ++i) {
    dCdt = dCdt + derivativeOfBernstein(i, N, t) * pList[i];
  }
  dT = gPoint(dCdt.x * cos(theta), dCdt.y, dCdt.x * sin(theta));
  dTheta = gPoint(-rValue.x * sin(theta), 0, rValue.x * cos(theta));
}


const ld BezierSurface::TINY = 0.01;

BezierSurface::BezierSurface(pointMatrix controllP_) :
        controllP(controllP_), M(controllP_.size() - 1), N(controllP_[0].size() - 1) {
  bounding = genBounding(0, 1, 0, 1);
}

bool BezierSurface::intersection(Line l, gPoint &I, gVector &n, UvParam &uv) {
  vector<ItsInfo> itsInfos = bounding->intersection(l);
  Mat M(3, 3, CV_64FC1);
  Mat F(3, 1, CV_64FC1);
  Mat X(3, 1, CV_64FC1);
  Mat X_(3, 1, CV_64FC1);
  for (vector<ItsInfo>::iterator it = itsInfos.begin(); it != itsInfos.end(); ++it) {
    ItsInfo itsInfo = *it;
    ld umin = itsInfo.info[0], umax = itsInfo.info[1], vmin = itsInfo.info[2], vmax = itsInfo.info[3];
    ld u = (umin + umax) / 2, v = (vmin + vmax) / 2, t = (itsInfo.P.z - l.P.z) / l.v.z;

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
      uv = UvParam(u, v);
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