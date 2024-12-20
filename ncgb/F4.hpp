#pragma once
#include <bits/stdc++.h>

#include "ambiguities.hpp"
#include "IdealMembershipStatus.hpp"
#include "matrix.hpp"
#include "reductions.hpp"

namespace ncgb {

template<typename K, typename X, class ord = DegLexOrd<X>>
std::vector<Poly<K, X, ord>> symbolicPreprocessing(const std::vector<Poly<K, X, ord>>& G, const std::vector<Poly<K, X, ord>>& P) {
  std::vector<Poly<K, X, ord>> res;

  std::set<Monomial<X>, ord> considered;
  std::vector<Monomial<X>> T;

  for (const auto& f : P) {
    if (!f.isZero()) {
      auto it = f.terms.rbegin();
      considered.insert(it->first);
      for (it++; it != f.terms.rend(); it++) {
        T.push_back(it->first);
        considered.insert(it->first);
      }
    }
  }

  while (!T.empty()) {
    Monomial<X> t = std::move(T.back());
    T.pop_back();

    for (const auto& g : G) {
      std::vector<std::pair<Monomial<X>, Monomial<X>>> div = g.lm().divide(t);
      if (!div.empty()) {
        Poly<K, X, ord> h = div[0].first * g * div[0].second;
        res.push_back(h);
        auto it = h.terms.rbegin();
        for (it++; it != h.terms.rend(); it++) {
          if (!considered.count(it->first)) {
            T.push_back(it->first);
            considered.insert(it->first);
          }
        }
        break;
      }
    }
  }

  return res;
}


template<typename K, typename X, class ord = DegLexOrd<X>>
std::pair<Matrix<K>, std::map<Monomial<X>, size_t, ord>> toMatrix(const std::vector<Poly<K, X, ord>>& G) {
  std::vector<Monomial<X>> T;
  for (const auto& f : G) {
    for (const auto& [m, c] : f.terms) {
      T.push_back(m);
    }
  }

  sort(T.begin(), T.end(), ord());

  std::map<Monomial<X>, size_t, ord> ids;

  for (size_t i = T.size(); i--; ) {
    if (i + 1 == T.size() || T[i] != T[i + 1]) {
      ids[T[i]] = ids.size();
    }
  }

  size_t n = G.size(), m = ids.size();

  Matrix<K> A(n, std::vector<K>(m));

  for (size_t i = 0; i < n; i++) {
    for (const auto& [m, c] : G[i].terms) {
      A[i][ids[m]] = c;
    }
  }

  return {A, ids};
}

template<typename K, typename X, class ord = DegLexOrd<X>>
std::vector<Poly<K, X, ord>> multiReduction(const std::vector<Poly<K, X, ord>>& G, const std::vector<Poly<K, X, ord>>& P) {
  std::vector<Poly<K, X, ord>> GG = symbolicPreprocessing(G, P);

  size_t GG_size = GG.size();

  for (const auto& f : P) {
    GG.push_back(f);
  }

  auto [A, ids] = toMatrix(GG);
  rref(A);

  std::vector<Monomial<X>> ids_inv(ids.size());
  for (const auto& [m, i] : ids) {
    ids_inv[i] = m;
  }

  std::vector<bool> GG_lms(ids.size(), false);
  for (size_t i = 0; i < GG_size; i++) {
    GG_lms[ids[GG[i].lm()]] = true;
  }

  std::vector<Poly<K, X, ord>> res;

  for (size_t i = 0; i < GG.size(); i++) {
    size_t j = 0;
    while (j < ids.size() && A[i][j] == K(0)) {
      j++;
    }
    if (j < ids.size() && !GG_lms[j]) {
      Poly<K, X, ord> f;
      for (; j < ids.size(); j++) {
        if (A[i][j] != K(0)) {
          f += Poly<K, X, ord>(ids_inv[j]) * A[i][j];
        }
      }
      res.push_back(f);
    }
  }

  return res;
}

template<typename K, typename X, class ord = DegLexOrd<X>>
std::vector<Poly<K, X, ord>> simplify(const std::vector<Poly<K, X, ord>>& G) {
  size_t n = G.size();

  auto [A, ids] = toMatrix(G);
  rref(A);

  std::vector<Monomial<X>> ids_inv(ids.size());
  for (const auto& [m, i] : ids) {
    ids_inv[i] = m;
  }

  std::vector<Poly<K, X, ord>> res;
  for (size_t i = 0; i < n; i++) {
    Poly<K, X, ord> f;
    for (size_t j = 0; j < ids.size(); j++) {
      if (A[i][j] != K(0)) {
        f += Poly<K, X, ord>(ids_inv[j]) * A[i][j];
      }
    }
    if (!f.isZero()) {
      res.push_back(f);
    }
  }

  return res;
}

// Struct for running the F4 algorithm
template<typename K, typename X, class ord = DegLexOrd<X>>
struct F4Incremental {

  std::vector<Poly<K, X, ord>> G;
  std::vector<std::vector<std::tuple<Amb<X>, size_t, size_t>>> ambs_per_deg;
  // We store ambiguities by their degree

  void add_poly(Poly<K, X, ord>& f) {
    G.push_back(std::move(f));
    size_t lim = G.size() - 1;
    std::vector<std::vector<std::tuple<Amb<X>, size_t, size_t>>> to_add(lim);

    #pragma omp parallel for
    for (size_t k = 0; k < lim; k++) {
      for (auto& amb : ambiguities(G[k].lm(), G.back().lm())) {
        if (!checkDeletionCriteria(G, amb, k, lim)) {
          to_add[k].push_back({std::move(amb), k, lim});
        }
      }
      for (auto& amb : ambiguities(G.back().lm(), G[k].lm())) {
        if (!checkDeletionCriteria(G, amb, lim, k)) {
          to_add[k].push_back({std::move(amb), lim, k});
        }
      }
    }

    for (size_t k = 0; k < lim; k++) {
      for (auto& [amb, i, j] : to_add[k]) {
        size_t d = amb.size();
        while (ambs_per_deg.size() <= d) {
          ambs_per_deg.push_back({});
        }
        ambs_per_deg[d].push_back({std::move(amb), i, j});
      }
    }
  }

  // Construct an element with the generator set
  F4Incremental(const std::vector<Poly<K, X, ord>>& GG) {
    std::vector<Poly<K, X, ord>> GG2 = simplify(GG);
    for (size_t i = 0; i < GG2.size(); i++) {
      add_poly(GG2[i]);
    }
  }

  // Continue computing the F4 algorithm until finding a new polynomial
  // Return empty vector if G is already a Gröbner base
  std::vector<Poly<K, X, ord>> next() {
    for (size_t k = 0; k < ambs_per_deg.size(); k++) {
      if (ambs_per_deg[k].empty()) {
        continue;
      }

      size_t n = G.size();

      std::vector<Poly<K, X, ord>> P;
      for (auto& [amb, i, j] : ambs_per_deg[k]) {
        P.push_back(S_poly(amb, G[i], G[j]));
      }

      ambs_per_deg[k].clear();

      std::vector<Poly<K, X, ord>> P_reduced = multiReduction(G, P);

      bool added = false;
      for (Poly<K, X, ord>& f : P_reduced) {
        reduce(f, G);
        if (!f.isZero()) {
          add_poly(f);
          added = true;
        }
      }

      if (added) {
        return {G.begin() + n, G.end()};
      }
    }

    return {};
  }

  // Compute the full Gröbner base. If there is now finite base this function will not halt
  std::vector<Poly<K, X, ord>> fullBase() {
    while (!next().empty()) {}
    return G;
  }
};

// Try yo check if f is in the ideal generated by G using st steps
template<typename K, typename X, class ord = DegLexOrd<X>>
IdealMembershipStatus inIdeal_F4(const std::vector<Poly<K, X, ord>>& G, Poly<K, X, ord> f, size_t st = 20) {
  F4Incremental<K, X, ord> bi(G);

  for (size_t i = G.size(); i < st;) {
    reduce(f, bi.G);
    if (f.isZero()) {
      return InIdeal;
    }
    std::vector<Poly<K, X, ord>> p = bi.next();
    if (p.empty()) {
      return NotInIdeal;
    }
    i += p.size();
  }

  return Unknown;
}

} // namespace ncgb
