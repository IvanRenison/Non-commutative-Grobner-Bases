#include <bits/stdc++.h>
using namespace std;

#include "../src/ambiguities.h"
#include "random_gens.h"

void test() {
  Monomial m0 = random_monomial(), m1 = random_monomial();
  HashInterval h0(m0.vals), h1(m1.vals);

  vector<Amb> ambs = ambiguities(m0, h0, m1, h1);

  for (auto& amb : ambs) {
    if (amb.type == Amb::Inclusion) {
      assert(m0.vals.size() >= m1.vals.size());

      Monomial m1_extra = amb.a * m1 * amb.b;
      assert(m1_extra == m0);
    } else {
      assert(!amb.a.empty());
      assert(!amb.b.empty());
      Monomial m0_extra = m0 * amb.b;
      Monomial m1_extra = amb.a * m1;
      assert(m0_extra == m1_extra);
    }
  }
}

int main() {
  for (uint i = 0; i < 1000; i++) {
    test();
  }

  cout << "Test passed!" << endl;
}
