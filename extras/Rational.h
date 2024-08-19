#pragma once

#include <bits/stdc++.h>
#include "BigInteger.h"

using namespace std;

// In case of overflow of __int128_t in fractions
//#pragma GCC optimize("trapv")

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



InfInt abs(InfInt x) {
  return x >= 0 ? x : -x;
}

InfInt my_gcd(InfInt a, InfInt b) {
  while (b != 0) {
    a = a % b;
    swap(a, b);
  }
  return a;
}

template <typename T> InfInt signum(T val) {
  return (T(0) < val) - (val < T(0));
}

//#define abs(x) ((x) >= 0 ? (x) : (-x))

struct Ratio {
  InfInt num;
  InfInt den;
  Ratio(InfInt num = 0, InfInt den = 1) {
    static int i = 0;
    i++;
    InfInt g = my_gcd(abs(num), den);
    this->num = num / (g);
    this->den = den / g;
  }

  Ratio operator+(Ratio o) const {
    InfInt new_den = den * o.den;
    InfInt new_num = num * o.den + o.num * den;
    return Ratio(new_num, new_den);
  }
  Ratio operator-(Ratio o) const {
    InfInt new_den = den * o.den;
    InfInt new_num = num * o.den - o.num * den;
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
    return Ratio(num * o.den * signum(o), den * (InfInt)abs(o.num));
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
    return num * o.den < o.num * den;
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
