#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "ambiguities.h"
#include "reductions.h"

template<typename K, class ord = DegLexOrd>
Poly<K, ord> S_poly(const Amb& amb, const Poly<K, ord>& f, const Poly<K, ord>& g) {
  return (amb.a * f * amb.b) / f.lc() - (amb.c * g * amb.d) / g.lc();
}

template<typename K, class ord = DegLexOrd>
struct BuchbergerIncremental {
  vector<Poly<K, ord>> G;
  vector<HashInterval> G_lm_hashes;
  deque<tuple<Amb, size_t, size_t>> ambs;
  size_t t = 0;

  BuchbergerIncremental(const vector<Poly<K, ord>>& GG) : G(GG) {
    for (auto& f : G) {
      G_lm_hashes.push_back(HashInterval(f.lm().vals));
    }
    for (size_t j = 0; j < G.size(); j++) {
      for (size_t i = 0; i <= j; i++) {
        for (auto& amb : ambiguities(G[i].lm(), G_lm_hashes[i], G[j].lm(), G_lm_hashes[j])) {
          add_amb(amb, i, j);
        }
      }
    }
  }

  void add_amb(Amb& amb, size_t i, size_t j) {
    ambs.push_back({move(amb), i, j});
  }

  optional<Poly<K, ord>> next() {
    if (t < G.size()) {
      Poly<K, ord> res = G[t];
      t++;
      return res;
    }
    while (!ambs.empty()) {
      auto [amb, i, j] = ambs.front();
      ambs.pop_front();

      Poly<K, ord> s = S_poly(amb, G[i], G[j]);
      s = reduce(s, G);

      if (!s.isZero()) {
        G.push_back(s);
        G_lm_hashes.push_back(HashInterval(s.lm().vals));
        for (size_t k = 0; k < G.size() - 1; k++) {
          for (auto& amb : ambiguities(G[k].lm(), G_lm_hashes[k], s.lm(), G_lm_hashes.back())) {
            add_amb(amb, k, G.size() - 1);
          }
        }
        t++;
        return s;
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
IdealMembershipStatus inIdeal(const vector<Poly<K, ord>>& G, Poly<K, ord> f, size_t max_sz = 20) {
  BuchbergerIncremental bi(G);

  for (size_t i = 0; i < max_sz; i++) {
    optional<Poly<K, ord>> p = bi.next();
    if (!p.has_value()) {
      return NotInIdeal;
    }
    f = reduce(f, bi.G);
    if (f.isZero()) {
      return InIdeal;
    }
  }

  return Unknown;
}
