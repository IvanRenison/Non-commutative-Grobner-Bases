#include <bits/stdc++.h>

#include "ncgb/ambiguities.hpp"
#include "random_gens.hpp"

using namespace std;
using namespace ncgb;

void test() {
  Monomial<X> m0 = random_monomial(), m1 = random_monomial();

  vector<Amb<X>> ambs = ambiguities(m0, m1);

  for (auto& amb : ambs) {
    if (amb.type == Amb<X>::Inclusion) {
      assert(m0.vals.size() >= m1.vals.size());

      Monomial<X> m1_extra = amb.a * m1 * amb.b;
      assert(m1_extra == m0);
    } else {
      assert(!amb.a.empty());
      assert(!amb.b.empty());
      Monomial<X> m0_extra = m0 * amb.b;
      Monomial<X> m1_extra = amb.a * m1;
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
