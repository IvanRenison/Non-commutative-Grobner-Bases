#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.cpp"


template<typename K, class ord = LexOrd>
optional<Poly<K, ord>>
reduce(Poly<K, ord>& f, const Poly<K, ord>& g, const Monomial& a, const Monomial& b) {
  Monomial m = a * g.lm() * b;

  if (!f.terms.count(m)) {
    return {};
  }

  return f - (a * g * b) * (f[m] / g.lc());
}

#include "../extras/Rational.cpp"

typedef Poly<Ratio> P;

int main() {
  P f, g;
  cin >> f >> g;
  Monomial a, b;
  cin >> a >> b;


  optional<P> q = reduce(f, g, a, b);
  if (!q.has_value()) {
    cout << "no ans" << endl;
  } else {
    cout << "ans:\n" << *q << endl;
  }

}

