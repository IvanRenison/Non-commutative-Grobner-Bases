#include <bits/stdc++.h>
using namespace std;

#include "../src/nc_monomial.h"
#include "random_gens.h"

typedef Monomial::X X;

bool slow_divides(const Monomial& m0, const Monomial& m1) {
  if (m0.vals.size() > m1.vals.size()) return false;
  for (size_t i = 0; i + m0.vals.size() <= m1.vals.size(); i++) {
    bool ok = true;
    for (size_t j = 0; j < m0.vals.size(); j++) {
      if (m0.vals[j] != m1.vals[i + j]) {
        ok = false;
        break;
      }
    }
    if (ok) return true;
  }
  return false;
}

void test_prod_and_divide() {

  Monomial m0 = random_monomial(), m1 = random_monomial(), m2 = random_monomial();

  Monomial prod = m0 * m1;

  assert(m0.divides(prod));
  assert(m1.divides(prod));

  Monomial m00 = m0;
  m00 *= m1;

  assert(prod == m00);

  prod *= m2;

  assert(m1.divides(prod));
  assert(m2.divides(prod));

  m00 *= m2;

  assert(prod == m00);

  optional<pair<Monomial, Monomial>> div = m1.divide(prod);
  assert(div.has_value());
}

void test_divide() {
  Monomial m0 = random_monomial(), m1 = random_monomial();

  bool div = slow_divides(m0, m1);
  assert(div == m0.divides(m1));

  if (div) {
    auto [a, b] = *m0.divide(m1);
    assert(a * m0 * b == m1);
  }
}

void test_IO() {
  Monomial m0 = random_monomial();

  string s;

  stringstream ss;
  ss << m0;

  Monomial m1;
  ss >> m1;

  assert(m0 == m1);
}


int main() {
  for (uint i = 0; i < 1000; i++) {
    test_prod_and_divide();
    test_divide();
    test_IO();
  }

  cout << "Test passed!" << endl;
}
