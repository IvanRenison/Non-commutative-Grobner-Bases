#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"


template<typename K, class ord = LexOrd>
optional<Poly<K, ord>>
reduce(Poly<K, ord>& f, const Poly<K, ord>& g, const Monomial& a, const Monomial& b) {
  Monomial m = a * g.lm() * b;

  if (!f.terms.count(m)) {
    return {};
  }

  return f - (a * g * b) * (f[m] / g.lc());
}
