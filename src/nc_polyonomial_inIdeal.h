#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"

/*
Store a polynomial as a linear combination in the monomials of a generator set
If the generator set is G, the polynomial is sum_({m0, i, m1, c} in terms) c * m0 * G[i] * m1
*/
template<typename K, class ord = DegLexOrd>
struct InIdealPoly {
  vector<tuple<Monomial, size_t, Monomial, K>> terms;

  InIdealPoly() {}
  void add(Monomial& m0, size_t i, Monomial& m1, K c) {
    if (c != K(0)) {
      terms.push_back({m0, i, m1, c});
    }
  }
  void add(Monomial m0, size_t i, Monomial m1, K c) {
    if (c != K(0)) {
      terms.push_back({m0, i, m1, c});
    }
  }
  void addMove(Monomial& m0, size_t i, Monomial& m1, K c) {
    if (c != K(0)) {
      terms.push_back({move(m0), i, move(m1), move(c)});
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

  InIdealPoly operator*(const Monomial& m) const {
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

  Poly<K, ord> construct(const vector<Poly<K, ord>>& G) const {
    Poly<K, ord> res;
    for (auto [m0, i, m1, c] : terms) {
      res += m0 * G[i] * m1 * c;
    }
    return res;
  }

  void nice_print(ostream& os = cout) const {
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

template<typename K, class ord = DegLexOrd>
InIdealPoly<K, ord> operator*(const Monomial& m, const InIdealPoly<K, ord>& p) {
  InIdealPoly<K, ord> res;
    for (auto& [m0, i, m1, c] : p.terms) {
      res.add(m * m0, i, m1, c);
    }
    return res;
}
