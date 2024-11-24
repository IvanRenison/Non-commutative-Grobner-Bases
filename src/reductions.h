#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"

/* Reduce inplace with one polynomial, return true iff the polynomial was reduced */
template<typename K, class ord = DegLexOrd>
bool reduce(Poly<K, ord>& f, const Poly<K, ord>& g) {
  if (f.isZero()) {
    return false;
  }
  if (g.isZero()) {
    return false;
  }

  const Monomial& gm = g.lm();
  K gc = g.lc();

  bool ans = false;
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
        ans = true;
        break;
      }
    }
    if (!found) {
      break;
    }
  }

  return ans;
}

/* Reduce in place with a vector of polynomials, return true iff the polynomial was reduced */
template<typename K, class ord = DegLexOrd>
void reduce(Poly<K, ord>& f, const vector<Poly<K, ord>>& G) {
  while (true) {
    bool red = false;
    for (const auto& g : G) {
      bool red_now = reduce(f, g);
      if (red_now) {
        red = true;
        break;
      }
    }
    if (!red) {
      break;
    }
  }
}

/* Reduce in place with a vector of polynomials, but only with the nor marked */
template<typename K, class ord = DegLexOrd>
void reduce(Poly<K, ord>& f, const vector<Poly<K, ord>>& G, const vector<bool> marks) {
  while (true) {
    bool red = false;
    for (size_t i = 0; i < G.size(); i++) if (!marks[i]) {
      bool red_now = reduce(f, G[i]);
      if (red_now) {
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
      reduce(G[i], G[j]);
      if (PolyOrd<K, ord>()(G[i], G[j])) {
        swap(G[i], G[j]);
        i--;
      }
    }
  }

  // Erase all zero polynomials
  G.erase(remove(G.begin(), G.end(), Poly<K, ord>()), G.end());
}
