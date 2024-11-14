#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;

#include "../src/nc_polynomial.h"
#include "random_gens.h"

typedef Poly<mpq_class> P;
typedef Monomial::X X;

void test_operations() {
  P p = random_poly(), q = random_poly(), r = random_poly();
  Monomial m = random_monomial();
  mpq_class c0 = random_rational(), c1 = random_rational();
  assert(p + q == q + p);
  assert(p + (q + r) == (p + q) + r);
  assert(p + P() == p);
  assert(p - p == P());
  assert(p - q == p + (-q));
  assert((p + q) - q == p);
  assert(p * (q * r) == (p * q) * r);
  assert(p * P() == P());
  assert(p * mpq_class(1) == p);
  assert(p * P(m) == p * m);
  assert(p * mpq_class(0) == P());
  assert((p + q) * m == p * m + q * m);
  assert((p + q) * r == p * r + q * r);
  assert(r * (p + q) == r * p + r * q);
  assert((p + q) * c0 == p * c0 + q * c0);
  assert((p * c0) * c1 == p * (c0 * c1));

  P pq = p * q;
  p *= q;
  assert(p == pq);
}

void test_IO() {
  P m0 = random_poly();

  stringstream ss;
  ss << m0;

  P m1;
  ss >> m1;

  assert(m0 == m1);
}


int main() {
  for (uint i = 0; i < 1000; i++) {
    test_operations();
    test_IO();
  }

  cout << "Test passed!" << endl;
}
