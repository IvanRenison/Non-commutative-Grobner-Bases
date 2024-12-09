#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "ambiguities.h"
#include "matrix.h"
#include "reductions.h"

template<typename K, class ord = DegLexOrd>
vector<Poly<K, ord>> symbolicPreprocessing(const vector<Poly<K, ord>>& G, const vector<Poly<K, ord>>& P) {
  vector<Poly<K, ord>> res;

  set<Monomial, ord> considered;
  vector<Monomial> T;

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
    Monomial t = move(T.back());
    T.pop_back();

    for (const auto& g : G) {
      vector<pair<Monomial, Monomial>> div = g.lm().divide(t);
      if (!div.empty()) {
        Poly<K, ord> h = div[0].first * g * div[0].second;
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


template<typename K, class ord = DegLexOrd>
pair<Matrix<K>, map<Monomial, size_t, ord>> toMatrix(const vector<Poly<K, ord>>& G) {
  vector<Monomial> T;
  for (const auto& f : G) {
    for (const auto& [m, c] : f.terms) {
      T.push_back(m);
    }
  }

  sort(T.begin(), T.end(), ord());

  map<Monomial, size_t, ord> ids;

  for (size_t i = T.size(); i--; ) {
    if (i + 1 == T.size() || T[i] != T[i + 1]) {
      ids[T[i]] = ids.size();
    }
  }

  size_t n = G.size(), m = ids.size();

  Matrix<K> A(n, vector<K>(m));

  for (size_t i = 0; i < n; i++) {
    for (const auto& [m, c] : G[i].terms) {
      A[i][ids[m]] = c;
    }
  }

  return {A, ids};
}

template<typename K, class ord = DegLexOrd>
vector<Poly<K, ord>> multiReduction(const vector<Poly<K, ord>>& G, const vector<Poly<K, ord>>& P) {
  vector<Poly<K, ord>> GG = symbolicPreprocessing(G, P);

  for (const auto& f : P) {
    GG.push_back(f);
  }

  auto [A, ids] = toMatrix(GG);
  rref(A);

  vector<Monomial> ids_inv(ids.size());
  for (const auto& [m, i] : ids) {
    ids_inv[i] = m;
  }

  vector<bool> GG_lms(ids.size(), false);
  for (const auto& f : GG) {
    GG_lms[ids[f.lm()]] = true;
  }

  vector<Poly<K, ord>> res;

  for (size_t i = 0; i < GG.size(); i++) {
    size_t j = 0;
    while (j < ids.size() && A[i][j] == K(0)) {
      j++;
    }
    if (j < ids.size() && !GG_lms[j]) {
      Poly<K, ord> f;
      for (; j < ids.size(); j++) {
        if (A[i][j] != K(0)) {
          f += Poly<K, ord>(ids_inv[j]) * A[i][j];
        }
      }
      res.push_back(f);
    }
  }

  return res;
}

template<typename K, class ord = DegLexOrd>
vector<Poly<K, ord>> simplify(const vector<Poly<K, ord>>& G) {
  size_t n = G.size();

  auto [A, ids] = toMatrix(G);
  rref(A);

  vector<Monomial> ids_inv(ids.size());
  for (const auto& [m, i] : ids) {
    ids_inv[i] = m;
  }

  vector<Poly<K, ord>> res;
  for (size_t i = 0; i < n; i++) {
    Poly<K, ord> f;
    for (size_t j = 0; j < ids.size(); j++) {
      if (A[i][j] != K(0)) {
        f += Poly<K, ord>(ids_inv[j]) * A[i][j];
      }
    }
    if (!f.isZero()) {
      res.push_back(f);
    }
  }

  return res;
}

template<typename K, class ord = DegLexOrd>
struct F4Incremental {

  vector<Poly<K, ord>> G;
  vector<vector<tuple<Amb, size_t, size_t>>> ambs_per_deg;
  // We store ambiguities by their degree

  void add_amb(Amb& amb, size_t i, size_t j) { // i and j are the indices of the polynomials in G
    if (checkDeletionCriteria(G, amb, i, j)) {
      return;
    }
    size_t d = amb.size();
    while (ambs_per_deg.size() <= d) {
      ambs_per_deg.push_back({});
    }
    ambs_per_deg[d].push_back({move(amb), i, j});
  }

  void add_poly(Poly<K, ord>& f) {
    for (size_t k = 0; k < G.size(); k++) {
      for (auto& amb : ambiguities(G[k].lm(), f.lm())) {
        add_amb(amb, k, G.size());
      }
      for (auto& amb : ambiguities(f.lm(), G[k].lm())) {
        add_amb(amb, G.size(), k);
      }
    }
    G.push_back(move(f));
  }

  F4Incremental(const vector<Poly<K, ord>>& GG) {
    vector<Poly<K, ord>> GG2 = simplify(GG);
    for (size_t i = 0; i < GG2.size(); i++) {
      add_poly(GG2[i]);
    }
  }

  vector<Poly<K, ord>> next() {
    for (size_t k = 0; k < ambs_per_deg.size(); k++) {
      if (ambs_per_deg[k].empty()) {
        continue;
      }

      size_t n = G.size();

      vector<Poly<K, ord>> P;
      for (auto& [amb, i, j] : ambs_per_deg[k]) {
        if (amb.type == Amb::Inclusion) {
          P.push_back(amb.a * G[j] * amb.b);
          P.push_back(G[i]);
        } else {
          P.push_back(G[i] * amb.b);
          P.push_back(amb.a * G[j]);
        }
      }

      ambs_per_deg[k].clear();

      vector<Poly<K, ord>> P_reduced = multiReduction(G, P);

      bool added = false;
      for (Poly<K, ord>& f : P_reduced) {
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

  vector<Poly<K, ord>> fullBase() {
    while (!next().empty()) {}
    return G;
  }
};

enum IdealMembershipStatus {
  InIdeal,
  NotInIdeal,
  Unknown
};

template<typename K, class ord = DegLexOrd>
IdealMembershipStatus inIdeal_F4(const vector<Poly<K, ord>>& G, Poly<K, ord> f, size_t max_sz = 20) {
  F4Incremental bi(G);

  for (size_t i = G.size(); i < max_sz;) {
    reduce(f, bi.G);
    if (f.isZero()) {
      return InIdeal;
    }
    vector<Poly<K, ord>> p = bi.next();
    if (p.empty()) {
      return NotInIdeal;
    }
    i += p.size();
  }

  return Unknown;
}

