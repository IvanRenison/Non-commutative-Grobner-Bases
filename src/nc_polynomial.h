#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_monomial.h"

template<typename K, class ord = DegLexOrd>
struct Poly {
  vector<pair<Monomial, K>> terms;
  // Terms is mantened sorted by monomial with respect to ord

  Poly() {}
  Poly(const Monomial& m, K c = K(1)) {
    if (c != K(0)) {
      terms.push_back({m, c});
    }
  }
  Poly(vector<pair<Monomial, K>> p) {
    sort(p.begin(), p.end(), [&](const auto& a, const auto& b) {
      return ord()(a.first, b.first);
    });
    // Convine repeated terms
    for (size_t i = 0; i < p.size(); i++) {
      if (!terms.empty() && p[i].first == terms.back().first) {
        terms.back().second += p[i].second;
        if (terms.back().second == K(0)) {
          terms.pop_back();
        }
      } else {
        terms.push_back(move(p[i]));
      }
    }
  }

  static Poly constructor(vector<pair<Monomial, K>>& p) {
    sort(p.begin(), p.end(), [&](const auto& a, const auto& b) {
      return ord()(a.first, b.first);
    });
    Poly res;
    // Convine repeated terms
    for (size_t i = 0; i < p.size(); i++) {
      if (!res.terms.empty() && p[i].first == res.terms.back().first) {
        res.terms.back().second += p[i].second;
        if (res.terms.back().second == K(0)) {
          res.terms.pop_back();
        }
      } else {
        res.terms.push_back(move(p[i]));
      }
    }
    return res;
  }

  bool operator==(const Poly& p) const {
    return terms == p.terms;
  }

  Poly operator+(const Poly& p) const {
    vector<pair<Monomial, K>> res = terms;
    res.insert(res.end(), p.terms.begin(), p.terms.end());
    return constructor(res);
  }
  Poly operator-(const Poly& p) const {
    vector<pair<Monomial, K>> res = p.terms;
    for (auto& [m, c] : res) {
      c = -c;
    }
    res.insert(res.end(), terms.begin(), terms.end());
    return constructor(res);
  }
  Poly operator*(const Poly& p) const {
    vector<pair<Monomial, K>> res;
    for (const auto& [m1, c1] : terms) {
      for (const auto& [m2, c2] : p.terms) {
        res.push_back({m1 * m2, c1 * c2});
      }
    }
    return constructor(res);
  }
  Poly operator*(Monomial m) const {
    Poly res = *this;
    for (auto& [m_, d] : res.terms) {
      m_ *= m;
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
  Poly operator/(K c) const {
    if (c == K(0)) {
      return Poly();
    }
    Poly res = *this;
    for (auto& [m, d] : res.terms) {
      d /= c;
    }
    return res;
  }
  Poly operator-() const {
    Poly res = *this;
    for (auto& [m, d] : res.terms) {
      d = -d;
    }
    return res;
  }

  void operator+=(const Poly& p) {
    terms.insert(terms.end(), p.terms.begin(), p.terms.end());
    *this = constructor(terms);
  }
  void operator-=(const Poly& p) {
    for (auto& [m, c] : p.terms) {
      terms.push_back({m, -c});
    }
    *this = constructor(terms);
  }
  void operator*=(const Poly& p) {
    *this = *this * p;
  }
  void operator*=(Monomial m) {
    for (auto& [m_, d] : terms) {
      m_ *= m;
    }
  }
  void operator*=(K c) {
    for (auto& [m, d] : terms) {
      d *= c;
    }
  }

  K coeff(const Monomial& m) const {
    auto it = lower_bound(terms.begin(), terms.end(), m, ord());
    return it == terms.end() || it->first != m ? K(0) : it->second;
  }

  const Monomial& lm() const {
    return terms.rbegin()->first;
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
  bool isZero() const {
    return terms.empty();
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
    vector<pair<Monomial, K>> terms(n);
    for (size_t i = 0; i < n; i++) {
      Monomial mon;
      K c;
      is >> c >> mon;
      terms[i] = {mon, c};
    }
    p = Poly::constructor(terms);
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
    vector<pair<Monomial, K>> terms;
    while (true) {
      K c;
      is >> c;
      Monomial m = Monomial::nice_read(is);
      terms.push_back({m, c});
      while (is.peek() == ' ') is.ignore();
      if (is.peek() != '+') break;
      is.ignore();
    }
    return Poly::constructor(terms);;
  }
};

template<typename K, class ord = DegLexOrd>
Poly<K, ord> operator*(Monomial m, const Poly<K, ord>& p) {
  Poly<K, ord> res = p;
  for (auto& [m_, d] : res.terms) {
    m_ = m * m_;
  }
  return res;
}

template<typename K, class ord = DegLexOrd>
struct PolyOrd {
  bool operator()(const Poly<K, ord>& p1, const Poly<K, ord>& p2) const {
    for (auto it1 = p1.terms.rbegin(), it2 = p2.terms.rbegin();
        it1 != p1.terms.rend() && it2 != p2.terms.rend();
        it1++, it2++) {
      if (it1->first != it2->first) {
        return it1->first < it2->first;
      }
    }

    return p1.terms.size() < p2.terms.size();
  }
};
