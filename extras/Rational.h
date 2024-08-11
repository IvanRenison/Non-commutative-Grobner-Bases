#pragma once

#include <bits/stdc++.h>

using namespace std;

// In case of overflow of __int128_t in fractions
#pragma GCC optimize("trapv")

istream& operator>>(istream& is, __uint128_t& value) {
  unsigned long long x;
  is >> x;
  value = x;
  return is;
}

ostream& operator<<(ostream& os, __uint128_t value) {
  if (value >= 10) {
    os << (value / 10);
  }
  os << (char)(value % 10 + '0');
  return os;
}


istream& operator>>(istream& is, __int128_t& value) {
  long long x;
  is >> x;
  value = x;
  return is;
}

ostream& operator<<(ostream& os, __int128_t value) {
  if (value < 0) {
    os << '-';
    value = -value;
  }
  if (value >= 10) {
    os << (value / 10);
  }
  os << (char)(value % 10 + '0');
  return os;
}



typedef __int128_t ll;
typedef __uint128_t ull;

template <typename T> ll signum(T val) {
  return (T(0) < val) - (val < T(0));
}

#define abs(x) ((x) >= 0 ? (x) : (-x))

struct Ratio {
  ll num;
  ull den;
  Ratio(ll num = 0, ull den = 1) {
    ull g = __gcd((ull)abs(num), den);
    this->num = num / ((ll)g);
    this->den = den / g;
  }

  Ratio operator+(Ratio o) const {
    ull new_den = den * o.den;
    ll new_num = num * (ll)o.den + o.num * (ll)den;
    return Ratio(new_num, new_den);
  }
  Ratio operator-(Ratio o) const {
    ull new_den = den * o.den;
    ll new_num = num * (ll)o.den - o.num * (ll)den;
    return Ratio(new_num, new_den);
  }
  Ratio operator-() const {
    return Ratio(-num, den);
  }
  Ratio operator*(Ratio o) const {
    return Ratio(num * o.num, den * o.den);
  }
  Ratio operator/(Ratio o) const {
    assert(o.num != 0);
    return Ratio(num * (ll)o.den * signum(o), den * (ull)abs(o.num));
  }

  void operator+=(Ratio o) {
    *this = *this + o;
  }
  void operator-=(Ratio o) {
    *this = *this - o;
  }
  void operator*=(Ratio o) {
    *this = *this * o;
  }
  void operator/=(Ratio o) {
    *this = *this / o;
  }

  bool operator==(Ratio o) const {
    return num == o.num && den == o.den;
  }
  bool operator!=(Ratio o) const {
    return !(*this == o);
  }
  bool operator<(Ratio o) const {
    return num * (ll)o.den < o.num * (ll)den;
  }
  bool operator<=(Ratio o) const {
    return *this < o || *this == o;
  }
  bool operator>(Ratio o) const {
    return o < *this;
  }
  bool operator>=(Ratio o) const {
    return o <= *this;
  }

  friend ostream& operator<<(ostream& os, Ratio r) {
    os << r.num;
    if (r.den != 1) {
      os << "/" << r.den;
    }
    return os;
  }
  friend istream& operator>>(istream& is, Ratio& r) {
    is >> r.num;
    r.den = 1;
    char c = is.peek();
    if (c == '/') {
      is.ignore();
      is >> r.den;
    }
    return is;
  }
};
#undef abs
