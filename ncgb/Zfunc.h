#pragma once
#include <bits/stdc++.h>

namespace ncgb {

/*
Z function
z[i] computes the length of the longest common prefix of s[i:] and s, except z[0] = 0.

For example: abacaba -> 0010301
*/
template<typename X>
std::vector<size_t> Z(const std::vector<X>& s) {
  size_t n = s.size();
  std::vector<size_t> z(n);
  int l = -1, r = -1;
  for (size_t i = 1; i < n; i++) {
    z[i] = (int)i >= r ? 0 : std::min((size_t)(r - i), z[i - l]);
    while (i + z[i] < n && s[i + z[i]] == s[z[i]])
      z[i]++;
    if ((int)(i + z[i]) > r)
      l = i, r = i + z[i];
  }
  return z;
}

/* Compte the Z function of the concatenation of two std::vectors */
template<typename X>
std::vector<size_t> Z(const std::vector<X>& s, const std::vector<X>& t) {
  size_t n = s.size(), m = t.size();

  auto S = [&](size_t i) -> X {
    return i < n ? s[i] : t[i - n];
  };

  std::vector<size_t> z(n + m);
  int l = -1, r = -1;
  for (size_t i = 1; i < n + m; i++) {
    z[i] = (int)i >= r ? 0 : std::min((size_t)(r - i), z[i - l]);
    while (i + z[i] < n + m && S(i + z[i]) == S(z[i]))
      z[i]++;
    if ((int)(i + z[i]) > r)
      l = i, r = i + z[i];
  }
  return z;
}

} // namespace ncgb
