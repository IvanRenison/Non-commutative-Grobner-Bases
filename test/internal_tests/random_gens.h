#include <bits/stdc++.h>
#include <gmpxx.h>

#include "ncgb/nc_polynomial.h"

using namespace std;
using namespace ncgb;

typedef Monomial::X X;

Monomial random_monomial(size_t x_max = 5, size_t n_max = 10) {
  uint n = rand() % n_max;
  vector<X> vals0(n);
  for (X& x : vals0) {
    x = rand() % x_max;
  }
  return Monomial(vals0);
}

typedef Poly<mpq_class> P;

mpq_class random_rational() {
  int num = rand() % 10 - 5, den = rand() % 5 + 1;
  mpq_class r(num, den);
  r.canonicalize();
  return r;
}

P random_poly(size_t x_max = 5, size_t n_max = 10, size_t mon_max = 10) {
  uint n = rand() % n_max;
  P p;
  for (uint i = 0; i < n; i++) {
    Monomial m = random_monomial(x_max, mon_max);
    mpq_class r = random_rational();
    p += P(m, r);
  }
  return p;
}
