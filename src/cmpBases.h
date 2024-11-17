#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "reductions.h"

/* Check if two Gröbner bases generate the same ideal */
template<typename K, class ord = DegLexOrd>
bool cmpBases(const vector<Poly<K, ord>>& G0, const vector<Poly<K, ord>>& G1) {
  for (const auto& f : G0) {
    if (!reduce(f, G1).isZero()) {
      return false;
    }
  }
  for (const auto& f : G1) {
    if (!reduce(f, G0).isZero()) {
      return false;
    }
  }
  return true;
}
