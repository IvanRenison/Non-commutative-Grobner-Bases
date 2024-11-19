#include <bits/stdc++.h>
using namespace std;

#include "reductions.h"
#include "random_gens.h"

typedef Poly<mpq_class> P;
typedef Monomial::X X;

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

int main() {
  for (uint i = 0; i < 1000; i++) {
    test_is_reduced();
  }

  cout << "Test passed!" << endl;
}
