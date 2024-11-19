#include <bits/stdc++.h>
using namespace std;

#include "nc_monomial.h"
#include "random_gens.h"

typedef Monomial::X X;

vector<size_t> slow_divide_indexes(const Monomial& m0, const Monomial& m1) {
  size_t n = m0.size(), m = m1.size();

  vector<size_t> ans;

  for (int d = 0; d + n <= m; d++) {
    bool ok = true;
    for (size_t i = 0; i < n; i++) {
      if (m0.vals[i] != m1.vals[d + i]) {
        ok = false;
        break;
      }
    }
    if (ok) {
      ans.push_back(d);
    }
  }

  return ans;
}

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

vector<pair<Monomial, Monomial>> slow_divide(const Monomial& m0, const Monomial& m1) {
  vector<size_t> is = slow_divide_indexes(m0, m1);
  vector<pair<Monomial, Monomial>> ans;
  for (size_t i : is) {
    Monomial a(vector(m1.vals.begin(), m1.vals.begin() + i));
    Monomial b(vector(m1.vals.begin() + i + m0.size(), m1.vals.end()));
    ans.push_back({a, b});
  }
  return ans;
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

  vector<pair<Monomial, Monomial>> divs = m1.divide(prod);
  assert(!divs.empty());
  vector<pair<Monomial, Monomial>> slow_divs = slow_divide(m1, prod);
  assert(divs == slow_divs);
}

void test_divide() {
  Monomial m0 = random_monomial(), m1 = random_monomial();

  bool div = slow_divides(m0, m1);
  assert(div == m0.divides(m1));

  vector<size_t> is = m0.divide_indexes(m1);
  vector<size_t> slow_is = slow_divide_indexes(m0, m1);

  assert(is == slow_is);

  vector<pair<Monomial, Monomial>> divs = m0.divide(m1);
  vector<pair<Monomial, Monomial>> slow_divs = slow_divide(m0, m1);

  assert(divs == slow_divs);

  for (auto [a, b] : divs) {
    Monomial m = a * m0 * b;
    assert(m == m1);
  }
}

void test_eq() {
  Monomial m = random_monomial();

  vector<size_t> divs = m.divide_indexes(m);
  assert(divs.size() == 1);
  assert(divs[0] == 0);
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
    test_eq();
  }

  cout << "Test passed!" << endl;
}
