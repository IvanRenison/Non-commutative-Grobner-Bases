#pragma once
#include <bits/stdc++.h>
using namespace std;

#include "reductions.h"

struct Amb {
  Monomial a, b, c, d;
};

template<typename K, class ord = LexOrd>
Poly<K, ord> S_poly(const Amb& amb, const Poly<K, ord>& f, const Poly<K, ord>& g) {
  return (amb.a * f * amb.b) / f.lc() - (amb.c * g * amb.d) / g.lc();
}

vector<Amb> ambiguities(const Monomial& p, const Monomial& q) {
  vector<Amb> res;

  size_t n = p.size(), m = q.size();

  for (int d = - m + 1; d < (int)n; d++) {
    // q start d positions "after" p

    bool valid = true;
    for (int i = max(0, d); i < (int)min(n, d + m); i++) {
      if (p.vals[i] != q.vals[i - d]) {
        valid = false;
        break;
      }
    }

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

template<typename K, class ord = LexOrd>
pair<vector<Poly<K, ord>>, bool> Buchberger(vector<Poly<K, ord>> G, size_t max_iter = 100) {

  queue<tuple<Amb, size_t, size_t>> ambs;
  for (size_t j = 0; j < G.size(); j++) {
    for (size_t i = 0; i <= j; i++) {
      for (const auto& amb : ambiguities(G[i].lm(), G[j].lm())) {
        ambs.push({amb, i, j});
      }
    }
  }

  for (; !ambs.empty() && max_iter > 0; max_iter--) {
    auto [amb, i, j] = ambs.front();
    ambs.pop();

    Poly<K, ord> s = S_poly(amb, G[i], G[j]);
    s = reduce(s, G);

    if (!s.isZero()) {
      G.push_back(s);
      for (size_t k = 0; k < G.size() - 1; k++) {
        for (const auto& amb : ambiguities(G[k].lm(), s.lm())) {
          ambs.push({amb, k, G.size() - 1});
        }
      }
    }
  }

  return {G, max_iter == 0};
}

template<typename K, class ord = LexOrd>
struct BuchbergerIncremental {
  vector<Poly<K, ord>> G;
  queue<tuple<Amb, size_t, size_t>> ambs;
  size_t t = 0;

  BuchbergerIncremental(const vector<Poly<K, ord>>& GG) : G(GG) {
    for (size_t j = 0; j < G.size(); j++) {
      for (size_t i = 0; i <= j; i++) {
        for (const auto& amb : ambiguities(G[i].lm(), G[j].lm())) {
          ambs.push({amb, i, j});
        }
      }
    }
  }

  optional<Poly<K, ord>> next() {
    if (t < G.size()) {
      Poly<K, ord> res = G[t];
      t++;
      return res;
    }
    while (!ambs.empty()) {
      auto [amb, i, j] = ambs.front();
      ambs.pop();

      Poly<K, ord> s = S_poly(amb, G[i], G[j]);
      s = reduce(s, G);

      if (!s.isZero()) {
        G.push_back(s);
        for (size_t k = 0; k < G.size() - 1; k++) {
          for (const auto& amb : ambiguities(G[k].lm(), s.lm())) {
            ambs.push({amb, k, G.size() - 1});
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

template<typename K, class ord = LexOrd>
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
