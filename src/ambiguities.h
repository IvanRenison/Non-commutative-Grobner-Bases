#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "nc_polynomial.h"
#include "Hashing.h"

struct Amb {
  const Monomial& p, q;
  enum Type { Inclusion, Overlap };
  Type type;
  size_t pos; // position where q starts in p
  Monomial a, b;

  size_t size() const {
    if (type == Inclusion) {
      return p.size();
    } else {
      return p.size() + b.size();
    }
  }
};

vector<Amb>
ambiguities(const Monomial& p, const HashInterval& p_hi, const Monomial& q, const HashInterval& q_hi) {
  vector<Amb> res;

  size_t n = p.size(), m = q.size();

  // Inclusion of q inside p
  vector<size_t> div_idxs = q.divide_indexes(p);
  for (size_t i : div_idxs) {
    Amb amb = {
      p, q,
      Amb::Inclusion,
      i,
      Monomial(vector(p.vals.begin(), p.vals.begin() + i)),
      Monomial(vector(p.vals.begin() + i + q.size(), p.vals.end()))
    };
    res.push_back(amb);
  }

  // Overlap p q
  for (size_t i = n >= m ? n - m + 1 : 1; i < n; i++) {

    // p[i:] = q[:n - i]
    bool valid = p_hi.hashInterval(i, n) == q_hi.hashInterval(0, n - i);

    if (!valid) continue;

    Amb amb = {
      p, q,
      Amb::Overlap,
      i,
      Monomial(vector(p.vals.begin(), p.vals.begin() + i)),
      Monomial(vector(q.vals.begin() + (n - i), q.vals.end()))
    };

    res.push_back(amb);
  }

  return res;
}
