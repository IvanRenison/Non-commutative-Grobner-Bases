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
  size_t size() const { return vals.size(); }
  // Returns the position of m.vals where this monomial divides m, or `numeric_limits<size_t>::max()` if it doesn't
  size_t divide_index(const Monomial& m) const {
    if (size() == 0) return 0;
    if (size() > m.size()) return numeric_limits<size_t>::max();

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

    return numeric_limits<size_t>::max();
  }
  bool divides(const Monomial& m) const {
    return divide_index(m) < numeric_limits<size_t>::max();
  }
  optional<pair<Monomial, Monomial>> divide(const Monomial& m) const {
    size_t i = divide_index(m);
    if (i == numeric_limits<size_t>::max()) return {};
    Monomial a(vector(m.vals.begin(), m.vals.begin() + i));
    Monomial b(vector(m.vals.begin() + i + size(), m.vals.end()));
    return {{a, b}};
  }

  friend ostream& operator<<(ostream& os, const Monomial& m) {
    os << m.size();
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
  static Monomial nice_read(istream& is = cin) {
    string s;
    is >> s;
    Monomial m;
    for (char c : s) {
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
