#pragma once
#include <bits/stdc++.h>
using namespace std;

struct Monomial {
  typedef unsigned short X;

  vector<X> vals;

  Monomial() {}
  Monomial(const vector<X>& vals) : vals(vals) {}

  bool operator==(const Monomial& m) const { return vals == m.vals; }
  Monomial operator*(const Monomial& m) const {
    Monomial res;
    res.vals = vals;
    res.vals.insert(res.vals.end(), m.vals.begin(), m.vals.end());
    return res;
  }
  Monomial operator*=(const Monomial& m) {
    vals.insert(vals.end(), m.vals.begin(), m.vals.end());
    return *this;
  }
  bool divides(const Monomial& m) const {
    if (vals.size() > m.vals.size()) return false;
    for (size_t i = 0; i + vals.size() < m.vals.size(); i++) {
      bool ok = true;
      for (size_t j = 0; j < vals.size(); j++) {
        if (vals[j] != m.vals[i + j]) {
          ok = false;
          break;
        }
      }
      if (ok) return true;
    }
    return false;
  }
  size_t size() const { return vals.size(); }

  friend ostream& operator<<(ostream& os, const Monomial& m) {
    os << m.vals.size();
    for (auto x : m.vals) {
      os << ' ' << x;
    }
    return os;
  }
  friend istream& operator>>(istream& is, Monomial& m) {
    size_t n;
    is >> n;
    m.vals.resize(n);
    for (auto& x : m.vals) {
      is >> x;
    }
    return is;
  }

  void nice_print(ostream& os = cout) const {
    for (size_t i = 0; i < vals.size(); i++) {
      assert(vals[i] < 26);
      os << (char)('a' + vals[i]);
    }
  }
};

struct LexOrd {
  bool operator()(const Monomial& a, const Monomial& b) const {
    return a.vals < b.vals;
  }
};

