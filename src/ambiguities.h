#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"

struct Amb {
  Monomial a, b, c, d;
};

vector<Amb> ambiguities(const Monomial& p, const Monomial& q) {
  vector<Amb> res;

  size_t n = p.size(), m = q.size();

  for (int d = - m + 1; d < (int)n; d++) {
    // q start d positions "after" p

    bool valid = true;
    for (int i = max(0, d); i < (int)min(n, d + m); i++) {
      if (p.vals[i] != q.vals[i - d]) {
        valid = false;
        break;
      }
    }

    if (!valid) continue;

    Amb amb = {
      Monomial(vector(q.vals.begin(), q.vals.begin() + max(0, -d))),
      Monomial(vector(q.vals.begin() + min(m, n - d), q.vals.end())),
      Monomial(vector(p.vals.begin(), p.vals.begin() + max(0, d))),
      Monomial(vector(p.vals.begin() + min(n, d + m), p.vals.end())),
    };

    res.push_back(amb);
  }

  return res;
}
