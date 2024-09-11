#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"
#include "Hashing.h"

struct Amb {
  Monomial a, b, c, d;
};

vector<Amb>
ambiguities(const Monomial& p, const HashInterval& p_hi, const Monomial& q, const HashInterval& q_hi) {
  vector<Amb> res;

  size_t n = p.size(), m = q.size();

  for (int d = - m + 1; d < (int)n; d++) {
    // q start d positions "after" p

    int l = max(0, d), r = min(n, d + m);

    // p[l:r] == q[l - d: r - d]

    bool valid = p_hi.hashInterval(l, r) == q_hi.hashInterval(l - d, r - d);

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
