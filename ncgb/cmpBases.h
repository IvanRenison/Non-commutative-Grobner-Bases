#pragma once
#include <bits/stdc++.h>

#include "reductions.h"

namespace ncgb {

/* Check if two Gröbner bases generate the same ideal */
template<typename K, class ord = DegLexOrd>
bool cmpBases(const std::vector<Poly<K, ord>>& G0, const std::vector<Poly<K, ord>>& G1) {
  for (auto f : G0) {
    reduce<K, DegLexOrd>(f, G1);
    if (!f.isZero()) {
      return false;
    }
  }
  for (auto f : G1) {
    reduce<K, DegLexOrd>(f, G0);
    if (!f.isZero()) {
      return false;
    }
  }
  return true;
}

} // namespace ncgb
