#pragma once
#include <bits/stdc++.h>

#include "nc_polynomial.hpp"

namespace ncgb {

/*
Store a polynomial as a linear combination in the monomials of a generator set
If the generator set is G, the polynomial is sum_({m0, i, m1, c} in terms) c * m0 * G[i] * m1

Used for showing how are de element of a Gröbner base constructed from the generator set
*/
template<typename K, typename X = __uint8_t, class ord = DegLexOrd<X>>
struct InIdealPoly {
  std::vector<std::tuple<Monomial<X>, size_t, Monomial<X>, K>> terms;

  InIdealPoly() {}
  void add(Monomial<X>& m0, size_t i, Monomial<X>& m1, K c) {
    if (c != K(0)) {
      terms.push_back({m0, i, m1, c});
    }
  }
  void add(Monomial<X> m0, size_t i, Monomial<X> m1, K c) {
    if (c != K(0)) {
      terms.push_back({m0, i, m1, c});
    }
  }
  void addMove(Monomial<X>& m0, size_t i, Monomial<X>& m1, K c) {
    if (c != K(0)) {
      terms.push_back({std::move(m0), i, std::move(m1), std::move(c)});
    }
  }

  void operator+=(const InIdealPoly& p) {
    terms.insert(terms.end(), p.terms.begin(), p.terms.end());
  }
  void operator-=(const InIdealPoly& p) {
    for (auto& [m0, i, m1, c] : p.terms) {
      add(m0, i, m1, -c);
    }
  }

  InIdealPoly operator*(const Monomial<X>& m) const {
    InIdealPoly res;
    for (auto& [m0, i, m1, c] : terms) {
      res.add(m0, i, m1 * m, c);
    }
    return res;
  }

  InIdealPoly operator*(const K& c) const {
    InIdealPoly res;
    for (auto& [m0, i, m1, c0] : terms) {
      res.add(m0, i, m1, c0 * c);
    }
    return res;
  }

  void operator*=(const K& c) {
    for (auto& [m0, i, m1, c0] : terms) {
      c0 *= c;
    }
  }

  // Construct the real polynomial
  Poly<K, X, ord> construct(const std::vector<Poly<K, X, ord>>& G) const {
    Poly<K, X, ord> res;
    for (auto [m0, i, m1, c] : terms) {
      res += m0 * G[i] * m1 * c;
    }
    return res;
  }

  void nice_print(std::ostream& os = std::cout) const {
    for (auto it = terms.begin(); it != terms.end(); it++) {
      auto [m0, i, m1, c] = *it;
      if (it != terms.begin()) {
        if (c >= K(0)) {
          os << " + ";
        } else {
          os << " - ";
        }
        if (abs(c) != K(1)) {
          os << abs(c) << ' ';
        }
        m0.nice_print(os);
        os << " G[" << i << "] ";
      } else {
        if (c != K(1)) {
          os << c << ' ';
        }
        os << " G[" << i << "] ";
      }
    }
    os << '\n';
  }
};

template<typename K, typename X, class ord>
InIdealPoly<K, X, ord> operator*(const Monomial<X>& m, const InIdealPoly<K, X, ord>& p) {
  InIdealPoly<K, X, ord> res;
    for (auto& [m0, i, m1, c] : p.terms) {
      res.add(m * m0, i, m1, c);
    }
    return res;
}

} // namespace ncgb
