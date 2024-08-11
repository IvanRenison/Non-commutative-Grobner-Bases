#include <bits/stdc++.h>
using namespace std;

#include "../src/nc_polynomial.h"
#include "../extras/Rational.h"

typedef Monomial::X X;

Monomial random_monomial() {
  uint n = rand() % 10;
  vector<X> vals0(n);
  for (X& x : vals0) {
    x = rand() % 5;
  }
  return Monomial(vals0);
}

typedef Poly<Ratio> P;

P random_poly() {
  uint n = rand() % 10;
  P p;
  for (uint i = 0; i < n; i++) {
    Monomial m = random_monomial();
    Ratio r(rand() % 10 - 5, rand() % 5 + 1);
    p += P(m, r);
  }
  return p;
}
