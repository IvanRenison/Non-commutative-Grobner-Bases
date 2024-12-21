#pragma once
#include <bits/stdc++.h>

#include "reductions.hpp"

namespace ncgb {

/* Check if two Gröbner bases generate the same ideal */
template<typename K, typename X, class ord>
bool cmpBases(const std::vector<Poly<K, X, ord>>& G0, const std::vector<Poly<K, X, ord>>& G1) {
  for (auto f : G0) {
    reduce(f, G1);
    if (!f.isZero()) {
      return false;
    }
  }
  for (auto f : G1) {
    reduce(f, G0);
    if (!f.isZero()) {
      return false;
    }
  }
  return true;
}

} // namespace ncgb
