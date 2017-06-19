//
// Created by zxcroon on 17-4-28.
//

#include "gmath.h"

ld det(ld a, ld b, ld c, ld d, ld e, ld f, ld g, ld h, ld i) {
  return a * e * i + b * f * g + c * d * h - c * e * g - b * d * i - a * f * h;
}

ld Bernstein(int i, int n, ld t) {
  if (i < 0 || n < i) {
    return 0;
  }
  ld nfact = 1, ifact = 1, nmifact = 1;
  for (int k = 1; k <= n; ++k) {
    nfact *= k;
    if (k == i) {
      ifact = nfact;
    }
    if (k == n - i) {
      nmifact = nfact;
    }
  }
  return nfact / (ifact * nmifact) * pow(double(t), double(i)) * pow(double(1 - t), double(n - i));
}

ld derivativeOfBernstein(int i, int n, ld t) {
  return n * (Bernstein(i - 1, n - 1, t) - Bernstein(i, n - 1, t));
}
