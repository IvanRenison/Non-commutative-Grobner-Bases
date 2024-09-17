#include <bits/stdc++.h>
#include <givaro/qfield.h>
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

typedef Poly<Givaro::Rational> P;

Givaro::Rational random_rational() {
  int num = rand() % 10 - 5, den = rand() % 5 + 1;
  Givaro::Rational r(num, den);
  return r;
}

P random_poly() {
  uint n = rand() % 10;
  P p;
  for (uint i = 0; i < n; i++) {
    Monomial m = random_monomial();
    Givaro::Rational r = random_rational();
    p += P(m, r);
  }
  return p;
}
