#include <bits/stdc++.h>
using namespace std;

#include "../src/nc_monomial.cpp"
#include "random_gens.h"

typedef Monomial::X X;

void test_prod_and_divide() {

  Monomial m0 = random_monomial(), m1 = random_monomial(), m2 = random_monomial();

  Monomial prod = m0 * m1;

  assert(m0.divides(prod));
  assert(m1.divides(prod));

  m0 *= m1;

  assert(prod == m0);

  prod *= m2;

  assert(m1.divides(prod));
  assert(m2.divides(prod));

  m0 *= m2;

  assert(prod == m0);
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
    test_IO();
  }

  cout << "Test passed!" << endl;
}
