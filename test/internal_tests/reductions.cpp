#include <bits/stdc++.h>

#include "ncgb/reductions.hpp"
#include "random_gens.hpp"

using namespace std;
using namespace ncgb;

void test_is_reduced() {
  P f = random_poly();
  size_t n = rand() % 5 + 1;
  vector<P> G(n);
  for (P& g : G) {
    while (g.isZero()) {
      g = random_poly();
    }
  }

  reduce(f, G);

  for (const P& g : G) {
    for (const auto& [m, c] : f.terms) {
      assert(!g.lm().divides(m));
    }
  }
}

void testCofactor_simple() {
  P f = random_poly();
  P g = random_poly();
  while (g.isZero()) {
    g = random_poly();
  }

  P f_copy = f;
  auto red = reduceCofactor(f, g);
  if (!red.has_value()) {
    assert(f == f_copy);
  } else {
    auto [a, b, c] = *red;
    assert(f == f_copy + a * g * b * c);
  }

  bool red2 = reduce(f_copy, g);
  assert(red2 == red.has_value());
  assert(f == f_copy);
}

void testCofactorSet() {
  P f = random_poly();
  size_t n = rand() % 5 + 1;
  vector<P> G(n);
  for (P& g : G) {
    while (g.isZero()) {
      g = random_poly();
    }
  }

  vector<CofactorPoly<mpq_class, X>> G_rec(n);
  for (size_t i = 0; i < n; i++) {
    G_rec[i].terms.push_back({Monomial<X>(), i, Monomial<X>(), mpq_class(1)});
  }

  P f_copy = f;
  auto red = reduceCofactor(f, G, G_rec);
  for (const P& g : G) {
    for (const auto& [m, c] : f.terms) {
      assert(!g.lm().divides(m));
    }
  }

  P f_reduced = f;

  for (auto [a, i, b, c] : red.terms) {
    f += a * G[i] * b * c;
  }

  assert(f == f_copy);

  reduce(f_copy, G);
  assert(f_reduced == f_copy);
}

int main() {
  for (uint i = 0; i < 1000; i++) {
    test_is_reduced();
    testCofactor_simple();
    testCofactorSet();
  }

  cout << "Test passed!" << endl;
}
