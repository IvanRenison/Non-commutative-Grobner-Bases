#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"


template<typename K, class ord = DegLexOrd>
Poly<K, ord>
reduce(Poly<K, ord> f, const Poly<K, ord>& g) {
  if (f.isZero()) {
    return f;
  }

  const Monomial& gm = g.lm();
  K gc = g.lc();

  while (true) {
    bool found = false;
    Monomial m;
    K c;
    for (auto& [fm, fc] : views::reverse(f.terms)) {
      if (gm.divides(fm)) {
        m = fm;
        c = fc;
        found = true;
        break;
      }
    }
    if (!found) {
      break;
    }
    auto [a, b] = *gm.divide(m);
    f -= (a * g * b) * (c / gc);
  }

  return f;
}

template<typename K, class ord = DegLexOrd>
Poly<K, ord>
reduce(Poly<K, ord> f, const vector<Poly<K, ord>>& G) {
  while (true) {
    bool red = false;
    for (const auto& g : G) {
      Poly<K, ord> f_ = reduce(f, g);
      if (f != f_) {
        f = f_;
        red = true;
        break;
      }
    }
    if (!red) {
      break;
    }
  }
  return f;
}
