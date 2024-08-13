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

template<typename K, class ord = LexOrd>
Poly<K, ord>
reduce(Poly<K, ord> f, const Poly<K, ord>& g) {
  Monomial gm = g.lm();
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
    f = f - (a * g * b) * (c / gc);
  }

  return f;
}
