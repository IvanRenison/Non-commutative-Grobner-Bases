#pragma once
#include <bits/stdc++.h>

#include "nc_polynomial.h"
namespace ncgb {

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

  Monomial lm() const {
    if (type == Inclusion) {
      return p;
    } else {
      return p * b;
    }
  }
};

std::vector<Amb>
ambiguities(const Monomial& p, const Monomial& q) {
  std::vector<Amb> res;

  size_t n = p.size(), m = q.size();

  // Inclusion of q inside p
  std::vector<size_t> div_idxs = q.divide_indexes(p);
  for (size_t i : div_idxs) {
    Amb amb = {
      p, q,
      Amb::Inclusion,
      i,
      Monomial(std::vector(p.vals.begin(), p.vals.begin() + i)),
      Monomial(std::vector(p.vals.begin() + i + q.size(), p.vals.end()))
    };
    res.push_back(std::move(amb));
  }

  std::vector<size_t> z = Z(q.vals, p.vals);

  // Overlap p q
  for (size_t i = n >= m ? n - m + 1 : 1; i < n; i++) {

    // p[i:] = q[:n - i]
    bool valid = z[m + i] >= n - i;

    if (!valid) continue;

    Amb amb = {
      p, q,
      Amb::Overlap,
      i,
      Monomial(std::vector(p.vals.begin(), p.vals.begin() + i)),
      Monomial(std::vector(q.vals.begin() + (n - i), q.vals.end()))
    };

    res.push_back(std::move(amb));
  }

  return res;
}

/* Check if amb with o satisfy the general deletion criteria (Corollary 4.35 of Hof20). Pay attention polynomial indexes also.
Returns true if amb does not have to be added */
bool checkGeneralDeletionCriteria(const Amb& amb, const Monomial& o) {
  return o.divides(amb.lm());
}

/* Check if amb with o satisfy the specifics deletion criteria (Corollary 4.36 and 4.47 of Hof20). Pay attention polynomial indexes also.
Returns true if amb does not have to be added */
bool checkSpecificsDeletionCriteria(const Amb& amb, const Monomial& o) {
  if (!checkGeneralDeletionCriteria(amb, o)) return false;
  if (amb.type == Amb::Inclusion) {
    if (o.divides(amb.a) || o.divides(amb.b)) {
      return true;
    }
    if (o.divides(amb.q) && !(amb.a.empty() && amb.b.empty())){
      return true;
    }
    if (amb.q.divides(o) && o.size() < amb.p.size()){
      return true;
    }
  } else {
    if (o.divides(amb.a) && o.divides(amb.b)) {
      Monomial m(std::vector(amb.p.vals.begin() + amb.pos, amb.p.vals.end()));
      if (o.divides(m)){
        return true;
      }
    }
    std::vector<std::pair<Monomial, Monomial>> divs = o.divide(amb.lm());
    for (auto& [L, R] : divs) {
      if (L.empty() && R.size() < amb.b.size()){
        return true;
      }
      if (!L.empty() && L.size() < amb.a.size()){
        return true;
      }
      if (L.size() >= amb.a.size() && !R.empty()){
        return true;
      }
    }
  }
  return false;
}

/* Returns true if amb does not have to be added */
template<typename K, class ord = DegLexOrd>
bool checkDeletionCriteria(std::vector<Poly<K, ord>>& G, Amb& amb, size_t i, size_t j) {
  if (i == j && amb.type == Amb::Inclusion) {
    return true;
  }
  size_t k = 0;
  for (; k < std::min(i, j); k++) {
    if (checkGeneralDeletionCriteria(amb, G[k].lm())) {
      return true;
    }
  }
  for (; k < j; k++) {
    if (checkSpecificsDeletionCriteria(amb, G[k].lm())) {
      return true;
    }
  }
  if (amb.type == Amb::Overlap) {
    for (; k < G.size(); k++) {
      if (checkSpecificsDeletionCriteria(amb, G[k].lm())) {
        return true;
      }
    }
  }
  return false;
}

} // namespace ncgb
