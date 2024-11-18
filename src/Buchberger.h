#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "ambiguities.h"
#include "reductions.h"

template<typename K, class ord = DegLexOrd>
Poly<K, ord> S_poly(const Amb& amb, const Poly<K, ord>& f, const Poly<K, ord>& g) {
  if (amb.type == Amb::Inclusion) {
    return f / f.lc() - (amb.a * g * amb.b) / g.lc();
  } else {
    return (f * amb.b) / f.lc() - (amb.a * g) / g.lc();
  }
}

template<typename K, class ord = DegLexOrd>
struct BuchbergerIncremental {
  vector<Poly<K, ord>> G;
  deque<tuple<Amb, size_t, size_t>> ambs;
  size_t t = 0;

  BuchbergerIncremental(const vector<Poly<K, ord>>& GG) : G(GG) {
    interReduce(G);
    for (size_t j = 0; j < G.size(); j++) {
      for (size_t i = 0; i < G.size(); i++) {
        for (auto& amb : ambiguities(G[i].lm(), G[j].lm())) {
          add_amb(amb, i, j);
        }
        if (i != j) {
          for (auto& amb : ambiguities(G[j].lm(), G[i].lm())) {
            add_amb(amb, j, i);
          }
        }
      }
    }
  }

  void add_amb(Amb& amb, size_t i, size_t j) {
    if (checkDeletionCriteria(G, amb, i, j)) {
      return;
    }

    ambs.push_back({move(amb), i, j});
  }

  optional<Poly<K, ord>> next() {
    if (t < G.size()) {
      Poly<K, ord> res = G[t];
      t++;
      return res;
    }
    while (!ambs.empty()) {
      auto [amb, i, j] = move(ambs.front());
      ambs.pop_front();

      Poly<K, ord> s = S_poly(amb, G[i], G[j]);
      reduce(s, G);

      if (!s.isZero()) {
        G.push_back(s);
        for (size_t k = 0; k < G.size() - 1; k++) {
          for (auto& amb : ambiguities(G[k].lm(), s.lm())) {
            add_amb(amb, k, G.size() - 1);
          }
          for (auto& amb : ambiguities(s.lm(), G[k].lm())) {
            add_amb(amb, G.size() - 1, k);
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

  for (size_t i = G.size(); i < max_sz; i++) {
    optional<Poly<K, ord>> p = bi.next();
    if (!p.has_value()) {
      return NotInIdeal;
    }
    reduce(f, bi.G);
    if (f.isZero()) {
      return InIdeal;
    }
  }

  return Unknown;
}
