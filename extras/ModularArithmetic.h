#pragma once
#include <bits/stdc++.h>
using namespace std;

#include <givaro/modular.h>

typedef Givaro::Modular<size_t> Field;

template <size_t mod>
const Field F = Field(mod);

template <size_t mod>
struct Mod {
  size_t x;

  Mod(size_t x) : x(x % mod) {}
  Mod() : x(0) {}
  Mod(const Mod& o) : x(o.x) {}

  Mod operator=(const Mod& o) {
    F<mod>.assign(x, o.x);
    return *this;
  }

  Mod operator+(const Mod& o) const {
    Mod res;
    F<mod>.add(res.x, x, o.x);
    return res;
  }
  Mod operator-(const Mod& o) const {
    Mod res;
    F<mod>.sub(res.x, x, o.x);
    return res;
  }
  Mod operator*(const Mod& o) const {
    Mod res;
    F<mod>.mul(res.x, x, o.x);
    return res;
  }
  Mod operator/(const Mod& o) const {
    Mod res;
    F<mod>.div(res.x, x, o.x);
    return res;
  }

  Mod& operator+=(const Mod& o) {
    F<mod>.addin(x, o.x);
    return *this;
  }
  Mod& operator-=(const Mod& o) {
    F<mod>.subin(x, o.x);
    return *this;
  }
  Mod& operator*=(const Mod& o) {
    F<mod>.mulin(x, o.x);
    return *this;
  }
  Mod& operator/=(const Mod& o) {
    F<mod>.divin(x, o.x);
    return *this;
  }
  Mod operator-() const {
    Mod res;
    F<mod>.neg(res.x, x);
    return res;
  }
  Mod operator++() {
    F<mod>.addin(x, 1);
    return *this;
  }
  Mod operator--() {
    F<mod>.subin(x, 1);
    return *this;
  }
  Mod pow(size_t e) const {
    Mod r = Mod(1);
    Mod b = *this;
    while (e > 0) {
      if (e & 1) {
        r *= b;
      }
      b *= b;
      e /= 2;
    }
    return r;
  }

  bool operator==(const Mod& o) const {
    return F<mod>.areEqual(x, o.x);
  }
  bool operator<(const Mod& o) const {
    return x < o.x;
  }
  bool operator>(const Mod& o) const {
    return x > o.x;
  }
  bool operator<=(const Mod& o) const {
    return x <= o.x;
  }
  bool operator>=(const Mod& o) const {
    return x >= o.x;
  }

  friend ostream& operator<<(ostream& os, const Mod& m) {
    os << m.x;
    return os;
  }
  friend istream& operator>>(istream& is, Mod& m) {
    is >> m.x;
    return is;
  }


  operator size_t() const {
    return x;
  }
};
