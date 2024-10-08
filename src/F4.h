#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "ambiguities.h"
#include "matrix.h"
#include "reductions.h"

template<typename K, class ord = DegLexOrd>
vector<Poly<K, ord>> symbolicPreprocessing(const vector<Poly<K, ord>>& G, const vector<Poly<K, ord>>& P) {
  vector<Poly<K, ord>> res;

  set<Monomial, ord> T, done;

  for (const auto& f : P) {
    if (!f.isZero()) {
      auto it = f.terms.begin();
      done.insert(it->first);
      for (it++; it != f.terms.end(); it++) {
        T.insert(it->first);
      }
    }
  }

  while (!T.empty()) {
    Monomial t = *T.begin();
    T.erase(T.begin());
    done.insert(t);

    for (const auto& g : G) {
      optional<pair<Monomial, Monomial>> div = g.lm().divide(t);
      if (div.has_value()) {
        Poly<K, ord> h = div->first * g * div->second;
        res.push_back(h);
        auto it = h.terms.begin();
        for (it++; it != h.terms.end(); it++) {
          if (!done.count(it->first)) {
            T.insert(it->first);
          }
        }
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
  vector<HashInterval> G_lm_hashes;
  vector<vector<tuple<Amb, size_t, size_t>>> ambs_per_deg;
  // We store ambiguities by their degree

  void add_amb(Amb& amb, size_t i, size_t j) { // i and j are the indices of the polynomials in G
    size_t d = amb.a.size() + amb.b.size() + G[i].lm().size();
    while (ambs_per_deg.size() <= d) {
      ambs_per_deg.push_back({});
    }
    ambs_per_deg[d].push_back({move(amb), i, j});
  }

  F4Incremental(const vector<Poly<K, ord>>& GG) {
    G = simplify(GG);

    for (auto& f : G) {
      G_lm_hashes.push_back(HashInterval(f.lm().vals));
    }

    for (size_t j = 0; j < G.size(); j++) {
      for (size_t i = 0; i <= j; i++) {
        vector<Amb> ambs_ij = ambiguities(G[i].lm(), G_lm_hashes[i], G[j].lm(), G_lm_hashes[j]);
        for (auto& amb : ambs_ij) {
          add_amb(amb, i, j);
        }
      }
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
        P.push_back(amb.a * G[i] * amb.b);
        P.push_back(amb.c * G[j] * amb.d);
      }

      ambs_per_deg[k].clear();

      vector<Poly<K, ord>> P_reduced = multiReduction(G, P);

      bool added = false;
      for (Poly<K, ord>& f : P_reduced) {

        for (size_t i = 0; i < G.size(); i++) {
          vector<Amb> ambs_if = ambiguities(G[i].lm(), G_lm_hashes[i], f.lm(), G_lm_hashes.back());
          for (auto& amb : ambs_if) {
            add_amb(amb, i, G.size() - 1);
          }
        }

        G_lm_hashes.push_back(HashInterval(f.lm().vals));
        G.push_back(move(f));
        added = true;
      }

      if (added) {
        return {G.begin() + n, G.end()};
      }
    }

    return {};
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

  for (size_t i = 0; i < max_sz; i++) {
    vector<Poly<K, ord>> p = bi.next();
    if (p.empty()) {
      return NotInIdeal;
    }
    f = reduce(f, bi.G);
    if (f.isZero()) {
      return InIdeal;
    }
  }

  return Unknown;
}

