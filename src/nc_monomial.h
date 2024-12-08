#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "Zfunc.h"

/* Struct for representing non commutative monomials */
struct Monomial {
  typedef __uint8_t X;

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
  void operator*=(const Monomial& m) {
    vals.insert(vals.end(), m.vals.begin(), m.vals.end());
  }
  size_t size() const { return vals.size(); }
  bool empty() const { return vals.empty(); }

  // Returns the positions of m.vals where this monomial divides m
  vector<size_t> divide_indexes(const Monomial& m) const {
    if (size() > m.size()) return {};
    vector<size_t> ans;
    if (size() == 0) {
      ans = vector<size_t>(m.size() + 1, 0);
      iota(ans.begin(), ans.end(), 0);
      return ans;
    }

    size_t n = size() + m.size();

    vector<size_t> z = Z(vals, m.vals);

    for (size_t i = size(); i < n; i++) {
      if (z[i] >= size()) {
        ans.push_back(i - size());
      }
    }

    return ans;
  }

  // Return the first position where this monomial divides m
  optional<size_t> one_divide_index(const Monomial& m) const {
    if (size() > m.size()) return {};
    if (size() == 0) {
      return 0;
    }

    size_t n = size() + m.size();

    auto S = [&](size_t i) -> X {
      return i < size() ? vals[i] : m.vals[i - size()];
    };

    // Using Z function idea
    vector<size_t> z(n);
    int l = -1, r = -1;
    for (size_t i = 1; i < n; i++) {
      z[i] = (int)i >= r ? 0 : min((size_t)r - i, z[i - l]);
      while (i + z[i] < n && S(i + z[i]) == S(z[i])) {
        z[i]++;
      }
      if ((int)(i + z[i]) > r) {
        l = i, r = i + z[i];
      }

      if (i >= size() && z[i] >= size()) {
        return i - size();
      }
    }

    return {};
  }

  // Does this monomial divides m?
  bool divides(const Monomial& m) const {
    return one_divide_index(m).has_value();
  }

  // Make all possible divisions of m by this monomial
  vector<pair<Monomial, Monomial>> divide(const Monomial& m) const {
    vector<size_t> is = divide_indexes(m);
    vector<pair<Monomial, Monomial>> ans;
    for (size_t i : is) {
      Monomial a(vector(m.vals.begin(), m.vals.begin() + i));
      Monomial b(vector(m.vals.begin() + i + size(), m.vals.end()));
      ans.push_back({a, b});
    }
    return ans;
  }

  optional<pair<Monomial, Monomial>> one_divide(const Monomial& m) const {
    auto i = one_divide_index(m);
    if (!i.has_value()) return {};
    Monomial a(vector(m.vals.begin(), m.vals.begin() + *i));
    Monomial b(vector(m.vals.begin() + *i + size(), m.vals.end()));
    return {{a, b}};
  }

  friend ostream& operator<<(ostream& os, const Monomial& m) {
    os << m.size();
    for (auto x : m.vals) {
      os << ' ' << (unsigned int)x;
    }
    return os;
  }
  friend istream& operator>>(istream& is, Monomial& m) {
    size_t n;
    is >> n;
    m.vals.resize(n);
    for (auto& x : m.vals) {
      unsigned int xx;
      is >> xx;
      x = xx;
    }
    return is;
  }

  void nice_print(ostream& os = cout) const {
    for (size_t i = 0; i < vals.size(); i++) {
      assert(vals[i] < 26);
      os << (char)('a' + vals[i]);
    }
  }
  static Monomial nice_read(istream& is = cin) {
    while (is.peek() == ' ') is.ignore();
    Monomial m;
    while (isalpha(is.peek())) {
      char c;
      is >> c;
      m.vals.push_back(c - 'a');
    }
    return m;
  }
};

struct LexOrd {
  bool operator()(const Monomial& a, const Monomial& b) const {
    return a.vals < b.vals;
  }
};

struct DegLexOrd {
  bool operator()(const Monomial& a, const Monomial& b) const {
    size_t n = a.vals.size(), m = b.vals.size();
    return n < m || (n == m && a.vals < b.vals);
  }
};

/* For each x in m replace x by news[x] */
Monomial replace(const Monomial& m, const vector<Monomial>& news) {
  Monomial res;
  for (auto x : m.vals) {
    res *= news[x];
  }
  return res;
}
