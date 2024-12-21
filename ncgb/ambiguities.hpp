#pragma once
#include <bits/stdc++.h>

#include "nc_polynomial.hpp"
namespace ncgb {

template<typename X>
struct Amb {
  const Monomial<X>& p, q;
  enum Type { Inclusion, Overlap };
  Type type;
  size_t pos; // position where q starts in p
  Monomial<X> a, b;

  size_t size() const {
    if (type == Inclusion) {
      return p.size();
    } else {
      return p.size() + b.size();
    }
  }

  Monomial<X> lm() const {
    if (type == Inclusion) {
      return p;
    } else {
      return p * b;
    }
  }
};

template<typename X>
std::vector<Amb<X>>
ambiguities(const Monomial<X>& p, const Monomial<X>& q) {
  std::vector<Amb<X>> res;

  size_t n = p.size(), m = q.size();

  // Inclusion of q inside p
  std::vector<size_t> div_idxs = q.divide_indexes(p);
  for (size_t i : div_idxs) {
    Amb<X> amb = {
      p, q,
      Amb<X>::Inclusion,
      i,
      Monomial<X>(std::vector(p.vals.begin(), p.vals.begin() + i)),
      Monomial<X>(std::vector(p.vals.begin() + i + q.size(), p.vals.end()))
    };
    res.push_back(std::move(amb));
  }

  std::vector<size_t> z = Z(q.vals, p.vals);

  // Overlap p q
  for (size_t i = n >= m ? n - m + 1 : 1; i < n; i++) {

    // p[i:] = q[:n - i]
    bool valid = z[m + i] >= n - i;

    if (!valid) continue;

    Amb<X> amb = {
      p, q,
      Amb<X>::Overlap,
      i,
      Monomial<X>(std::vector(p.vals.begin(), p.vals.begin() + i)),
      Monomial<X>(std::vector(q.vals.begin() + (n - i), q.vals.end()))
    };

    res.push_back(std::move(amb));
  }

  return res;
}

template<typename K, typename X, class ord>
Poly<K, X, ord> S_poly(const Amb<X>& amb, const Poly<K, X, ord>& f, const Poly<K, X, ord>& g) {
  if (amb.type == Amb<X>::Inclusion) {
    return f / f.lc() - (amb.a * g * amb.b) / g.lc();
  } else {
    return (f * amb.b) / f.lc() - (amb.a * g) / g.lc();
  }
}

/* Check if amb with o satisfy the general deletion criteria (Corollary 4.35 of Hof20). Pay attention polynomial indexes also.
Returns true if amb does not have to be added */
template<typename X>
bool checkGeneralDeletionCriteria(const Amb<X>& amb, const Monomial<X>& o) {
  return o.divides(amb.lm());
}

/* Check if amb with o satisfy the specifics deletion criteria (Corollary 4.36 and 4.47 of Hof20). Pay attention polynomial indexes also.
Returns true if amb does not have to be added */
template<typename X>
bool checkSpecificsDeletionCriteria(const Amb<X>& amb, const Monomial<X>& o) {
  if (!checkGeneralDeletionCriteria(amb, o)) return false;
  if (amb.type == Amb<X>::Inclusion) {
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
      Monomial<X> m(std::vector<X>(amb.p.vals.begin() + amb.pos, amb.p.vals.end()));
      if (o.divides(m)){
        return true;
      }
    }
    std::vector<std::pair<Monomial<X>, Monomial<X>>> divs = o.divide(amb.lm());
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
template<typename K, typename X, class ord>
bool checkDeletionCriteria(std::vector<Poly<K, X, ord>>& G, Amb<X>& amb, size_t i, size_t j) {
  if (i == j && amb.type == Amb<X>::Inclusion) {
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
  if (amb.type == Amb<X>::Overlap) {
    for (; k < G.size(); k++) {
      if (checkSpecificsDeletionCriteria(amb, G[k].lm())) {
        return true;
      }
    }
  }
  return false;
}

} // namespace ncgb
