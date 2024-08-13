#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_monomial.h"

template<typename K, class ord = LexOrd>
struct Poly {
  map<Monomial, K, ord> terms;

  Poly() {}
  Poly(const Monomial& m, K c = 1) { terms[m] = c; }
  Poly(const map<Monomial, K, ord>& p) : terms(p) {}

  bool operator==(const Poly& p) const {
    return terms == p.terms;
  }

  Poly operator+(const Poly& p) const {
    Poly res = *this;
    for (const auto& [m, c] : p.terms) {
      res.terms[m] += c;
      if (res.terms[m] == K(0)) res.terms.erase(m);
    }
    return res;
  }
  Poly operator-(const Poly& p) const {
    Poly res = *this;
    for (const auto& [m, c] : p.terms) {
      res.terms[m] -= c;
      if (res.terms[m] == K(0)) res.terms.erase(m);
    }
    return res;
  }
  Poly operator*(const Poly& p) const {
    Poly res;
    for (const auto& [m1, c1] : terms) {
      for (const auto& [m2, c2] : p.terms) {
        res.terms[m1 * m2] += c1 * c2;
      }
    }
    for (auto it = res.terms.begin(); it != res.terms.end();) {
      if (it->second == K(0)) {
        it = res.terms.erase(it);
      } else {
        it++;
      }
    }
    return res;
  }
  Poly operator*(Monomial m) const {
    Poly res;
    for (const auto& [m_, d] : terms) {
      res.terms[m_ * m] = d;
    }
    return res;
  }
  Poly operator*(K c) const {
    if (c == K(0)) {
      return Poly();
    }
    Poly res = *this;
    for (auto& [m, d] : res.terms) {
      d *= c;
    }
    return res;
  }
  Poly operator-() const {
    Poly res;
    for (const auto& [m, c] : terms) {
      res.terms[m] = -c;
    }
    return res;
  }

  Poly operator+=(const Poly& p) {
    for (auto& [m, c] : p.terms) {
      terms[m] += c;
      if (terms[m] == 0) terms.erase(m);
    }
    return *this;
  }
  Poly operator-=(const Poly& p) {
    for (auto& [m, c] : p.terms) {
      terms[m] -= c;
      if (terms[m] == 0) terms.erase(m);
    }
    return *this;
  }
  Poly operator*=(const Poly& p) {
    return *this = *this * p;
  }
  Poly operator*=(Monomial m) {
    return *this = *this * m;
  }
  Poly operator*=(K c) {
    for (auto& [m, d] : terms) {
      d *= c;
    }
    return *this;
  }

  K coeff(const Monomial& m) const {
    auto it = terms.find(m);
    return it == terms.end() ? K(0) : it->second;
  }

  Monomial lm() const {
    if (terms.empty()) return Monomial();
    else return terms.rbegin()->first;
  }
  K lc() const {
    if (terms.empty()) return K(0);
    else return terms.rbegin()->second;
  }
  Poly lt() const {
    if (terms.empty()) return Poly();
    else return Poly({*terms.rbegin()});
  }
  bool monic() const {
    return lc() == K(1);
  }

  K operator[](const Monomial& m) {
    return terms[m];
  }

  friend ostream& operator<<(ostream& os, const Poly& p) {
    os << p.terms.size() << '\n';
    for (const auto& [m, c] : p.terms) {
      os << c << ' ' << m << '\n';
    }
    return os;
  }
  friend istream& operator>>(istream& is, Poly& p) {
    size_t n;
    is >> n;
    for (size_t i = 0; i < n; i++) {
      Monomial mon;
      K c;
      is >> c >> mon;
      p.terms[mon] = c;
    }
    return is;
  }

  void nice_print(ostream& os = cout) const {
    for (auto it = terms.begin(); it != terms.end(); it++) {
      if (it != terms.begin()) {
        os << " + ";
      }
      if (it->second != K(1)) {
        os << it->second << ' ';
      }
      it->first.nice_print(os);
    }
    os << '\n';
  }
  static Poly nice_read(istream& is = cin) {
    Poly res;
    while (true) {
      K c;
      is >> c;
      Monomial m = Monomial::nice_read(is);
      res.terms[m] = c;
      while (is.peek() == ' ') is.ignore();
      if (is.peek() != '+') break;
      is.ignore();
    }
    return res;
  }
};

template<typename K, class ord = LexOrd>
Poly<K, ord> operator*(Monomial m, const Poly<K, ord>& p) {
  Poly<K, ord> res;
  for (const auto& [m_, d] : p.terms) {
    res.terms[m * m_] = d;
  }
  return res;
}
