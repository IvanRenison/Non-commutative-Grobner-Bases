#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;

#include "../src/nc_polynomial.h"

typedef Monomial::X X;

Monomial random_monomial() {
  uint n = rand() % 10;
  vector<X> vals0(n);
  for (X& x : vals0) {
    x = rand() % 5;
  }
  return Monomial(vals0);
}

typedef Poly<mpq_class> P;

mpq_class random_mpq_class() {
  int num = rand() % 10 - 5, den = rand() % 5 + 1;
  mpq_class r(num, den);
  r.canonicalize();
  return r;
}

P random_poly() {
  uint n = rand() % 10;
  P p;
  for (uint i = 0; i < n; i++) {
    Monomial m = random_monomial();
    mpq_class r = random_mpq_class();
    p += P(m, r);
  }
  return p;
}
