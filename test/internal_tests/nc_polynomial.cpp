#include <bits/stdc++.h>
#include <gmpxx.h>

#include "ncgb/nc_polynomial.h"
#include "random_gens.h"

using namespace std;
using namespace ncgb;

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

void test_replace() {
  size_t x_max = rand() % 3 + 1;

  P p = random_poly(x_max);
  vector<P> ids(x_max);
  for (size_t i = 0; i < x_max; i++) {
    ids[i] = P(Monomial({(X)i}));
  }

  P q = replace(p, ids);

  assert(p == q);

  vector<P> news0(x_max), news1(x_max);
  for (size_t i = 0; i < x_max; i++) {
    news0[i] = random_poly(x_max, 2, 2);
    news1[i] = random_poly(x_max, 2, 2);
  }

  vector<P> comp(x_max);
  for (size_t i = 0; i < x_max; i++) {
    comp[i] = replace(news0[i], news1);
  }

  P p0 = replace(p, news0);
  p0 = replace(p0, news1);
  P p1 = replace(p, comp);

  assert(p0 == p1);
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
    test_replace();
  }

  cout << "Test passed!" << endl;
}
