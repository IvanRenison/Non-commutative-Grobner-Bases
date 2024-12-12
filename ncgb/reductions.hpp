#pragma once
#include <bits/stdc++.h>

#include "nc_polynomial.hpp"
#include "nc_polyonomial_inIdeal.hpp"

namespace ncgb {

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

  for (auto& [fm, fc] : std::views::reverse(f.terms)) {
    auto divs = gm.one_divide(fm);
    if (divs.has_value()) {
      auto& [a, b] = *divs;
      f -= (a * g * b) * (fc / gc);
      return true;
    }
  }

  return false;
}

/* Reduce in place with a std::vector of polynomials, but only with the nor marked */
template<typename K, class ord = DegLexOrd>
void reduce(Poly<K, ord>& f, const std::vector<Poly<K, ord>>& G, const std::vector<bool> marks) {
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

/* Reduce in place with a std::vector of polynomials, return true iff the polynomial was reduced */
template<typename K, class ord = DegLexOrd>
void reduce(Poly<K, ord>& f, const std::vector<Poly<K, ord>>& G) {
  std::vector<bool> marks(G.size(), false);
  reduce(f, G, marks);
}

/* Reduces several polynomials with each other */
template<typename K, class ord = DegLexOrd>
void interReduce(std::vector<Poly<K, ord>>& G) {
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

/* Reduce inplace with one polynomial, return how the reduction was made (or null if no reduction was made)*/
template<typename K, class ord = DegLexOrd>
std::optional<std::tuple<Monomial, Monomial, K>> reduceReconstruct(Poly<K, ord>& f, const Poly<K, ord>& g) {
  if (f.isZero()) {
    return {};
  }
  if (g.isZero()) {
    return {};
  }

  const Monomial& gm = g.lm();
  K gc = g.lc();

  for (auto& [fm, fc] : std::views::reverse(f.terms)) {
    auto divs = gm.one_divide(fm);
    if (divs.has_value()) {
      auto& [a, b] = *divs;
      K c = - fc / gc;
      f += (a * g * b) * c;
      return {{a, b, c}};
    }
  }

  return {};
}

/* Reduce in place with a std::vector of polynomials, but only with the nor marked, returning how the reduction was made */
template<typename K, class ord = DegLexOrd>
InIdealPoly<K, ord>
reduceReconstruct(Poly<K, ord>& f, const std::vector<Poly<K, ord>>& G, const std::vector<InIdealPoly<K, ord>>& G_rec, const std::vector<bool> marks) {
  InIdealPoly<K, ord> res;
  while (true) {
    bool red = false;
    for (size_t i = 0; i < G.size(); i++) if (!marks[i]) {
      auto red_now = reduceReconstruct(f, G[i]);
      if (red_now.has_value()) {
        auto& [a, b, c] = *red_now;
        for (const auto& [a_, i_, b_, c_] : G_rec[i].terms) {
          res.add(a * a_, i_, b_ * b, -c * c_);
        }
        red = true;
        break;
      }
    }
    if (!red) {
      break;
    }
  }
  return res;
}

/* Reduce in place with a std::vector of polynomials returning how the reduction was made */
template<typename K, class ord = DegLexOrd>
InIdealPoly<K, ord>
reduceReconstruct(Poly<K, ord>& f, const std::vector<Poly<K, ord>>& G, const std::vector<InIdealPoly<K, ord>>& g_rec) {
  std::vector<bool> marks(G.size(), false);
  return reduceReconstruct(f, G, g_rec, marks);
}

} // namespace ncgb
