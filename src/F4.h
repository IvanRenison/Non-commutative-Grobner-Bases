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
  set<Monomial, ord> T;
  for (const auto& f : G) {
    for (const auto& [m, c] : f.terms) {
      T.insert(m);
    }
  }

  map<Monomial, size_t, ord> ids;

  for (const auto& m : views::reverse(T)) {
    ids[m] = ids.size();
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
/*   cerr << "multiReduction" << endl;
  cerr << "G:" << endl;
  for (const auto& f : G) {
    f.nice_print();
  }
  cerr << "P:" << endl;
  for (const auto& f : P) {
    f.nice_print();
  } */
  vector<Poly<K, ord>> GG = symbolicPreprocessing(G, P);
/*   cerr << "GG:" << endl;
  for (const auto& f : GG) {
    f.nice_print();
  } */

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
/*       cerr << "Adding ";
      f.nice_print(cerr); */
    }
  }

  return res;
}


template<typename K, class ord = DegLexOrd>
struct F4Incremental {

  vector<Poly<K, ord>> G;
  vector<tuple<Amb, size_t, size_t>> ambs, new_ambs;
  size_t k = 0;

  void sort_ambs() {
    sort(ambs.begin(), ambs.end(), [&](const tuple<Amb, size_t, size_t>& a, const tuple<Amb, size_t, size_t>& b) {
      auto [amb_a, i_a, j_a] = a;
      auto [amb_b, i_b, j_b] = b;

      Monomial lm_a = amb_a.a * G[i_a].lm() * amb_a.b;
      Monomial lm_b = amb_b.a * G[i_b].lm() * amb_b.b;

      if (lm_a == lm_b) {
        return pair{i_a, j_a} < pair{i_b, j_b};
      } else {
        return ord()(lm_a, lm_b);
      }
    });
  }

  F4Incremental(const vector<Poly<K, ord>>& GG) : G(GG) {
    for (size_t j = 0; j < G.size(); j++) {
      for (size_t i = 0; i <= j; i++) {
        for (const auto& amb : ambiguities(G[i].lm(), G[j].lm())) {
          ambs.push_back({amb, i, j});
        }
      }
    }

    sort_ambs();
  }

  vector<Poly<K, ord>> next() {
    while (true) {
      size_t n = G.size();
      //cerr << "n = " << n << endl;

      vector<Poly<K, ord>> P;
      for (size_t l = 0; l < 1 && k < ambs.size(); k++, l++) {
        auto& [amb, i, j] = ambs[k];
        P.push_back(amb.a * G[i] * amb.b);
        P.push_back(amb.c * G[j] * amb.d);
      }

      vector<Poly<K, ord>> P_reduced = multiReduction(G, P);

      bool added = false;
      for (const auto& f : P_reduced) {
        for (size_t k = 0; k < G.size(); k++) {
          for (const auto& amb : ambiguities(G[k].lm(), f.lm())) {
            new_ambs.push_back({amb, k, G.size()});
          }
        }

        G.push_back(f);
        added = true;
      }

      if (k == ambs.size()) {
        ambs = move(new_ambs);
        k = 0;
        sort_ambs();
      }

      if (added) {
        return {G.begin() + n, G.end()};
      }
      if (ambs.empty()) {
        return {};
      }
    }
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
    //cerr << "i = " << i << endl;
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

