#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"

/* Reduce inplace with one polynomial */
template<typename K, class ord = DegLexOrd>
Poly<K, ord> reduce(Poly<K, ord> f, const Poly<K, ord>& g) {
  if (f.isZero()) {
    return f;
  }
  if (g.isZero()) {
    return f;
  }

  const Monomial& gm = g.lm();
  K gc = g.lc();

  while (true) {
    bool found = false;
    Monomial m;
    K c;
    for (auto& [fm, fc] : views::reverse(f.terms)) {
      auto divs = gm.one_divide(fm);
      if (divs.has_value()) {
        auto& [a, b] = *divs;
        f -= (a * g * b) * (fc / gc);
        found = true;
        break;
      }
    }
    if (!found) {
      break;
    }
  }

  return f;
}

/* Reduce in place with a vector of polynomials */
template<typename K, class ord = DegLexOrd>
void reduce(Poly<K, ord>& f, const vector<Poly<K, ord>>& G) {
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
}

/* Reduces several polynomials with each other */
template<typename K, class ord = DegLexOrd>
void interReduce(vector<Poly<K, ord>>& G) {
  size_t n = G.size();
  sort(G.begin(), G.end(), PolyOrd<K, ord>());

  for (size_t i = 0; i < n; i++) {
    for (size_t j = i; j--; ) {
      G[i] = reduce(G[i], G[j]);
      if (PolyOrd<K, ord>()(G[i], G[j])) {
        swap(G[i], G[j]);
        i--;
      }
    }
  }

  // Erase all zero polynomials from the start
  G.erase(remove(G.begin(), G.end(), Poly<K, ord>()), G.end());
}
