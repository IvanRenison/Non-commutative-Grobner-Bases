#include <bits/stdc++.h>

#include "ncgb/Buchberger.hpp"
#include "random_gens.hpp"

using namespace std;
using namespace ncgb;

void testS_poly() {
  P f = random_poly();
  while (f.isZero()) {
    f = random_poly();
  }
  P g = random_poly();
  while (g.isZero()) {
    g = random_poly();
  }
  auto ambs = ambiguities(f.lm(), g.lm());

  for (auto amb : ambs) {
    P s = S_poly(amb, f, g);
    auto [s_rec, f_rec, g_rec] = S_polyCofactor(amb, f, g);

    if (s != s_rec) {
      f.nice_print(cerr);
      g.nice_print(cerr);
      s.nice_print(cerr);
      s_rec.nice_print(cerr);
      assert(false);
    }
    auto [fa, fb, fc] = f_rec;
    auto [ga, gb, gc] = g_rec;
    assert(s == fa * f * fb * fc + ga * g * gb * gc);
  }
}

void testInIdeal() {
  P f = random_poly();
  size_t n = rand() % 1 + 1;
  vector<P> G(n);
  for (P& g : G) {
    while (g.isZero()) {
      g = random_poly();
    }
  }

  P f_copy = f;
  auto [st, rec] = inIdealCofactor(G, f, 20);

  if (st == InIdeal) {
    P f2 = rec->construct(G);
    assert(f2 == f_copy);
  }

  IdealMembershipStatus st2 = inIdeal(G, f_copy, 20);

  assert(st == st2);
}


int main() {
  for (uint i = 0; i < 1000; i++) {
    testInIdeal();
    testS_poly();
  }

  cout << "Test passed!" << endl;
}
