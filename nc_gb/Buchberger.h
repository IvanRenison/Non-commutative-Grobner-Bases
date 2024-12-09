#pragma once
#include <bits/stdc++.h>

#include "ambiguities.h"
#include "reductions.h"

namespace nc_gb {

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
  std::vector<Poly<K, ord>> G;
  std::vector<bool> removed;
  std::deque<std::tuple<Amb, size_t, size_t>> ambs;
  size_t t = 0;

  BuchbergerIncremental(std::vector<Poly<K, ord>> GG) {
    for (size_t i = 0; i < GG.size(); i++) {
      add_poly(GG[i]);
    }
  }

  void add_amb(Amb& amb, size_t i, size_t j) {
    if (checkDeletionCriteria(G, amb, i, j)) {
      return;
    }

    ambs.push_back({std::move(amb), i, j});
  }

  void add_poly(const Poly<K, ord>& f) {
    G.push_back(f);
    removed.push_back(false);
    for (size_t k = 0; k < G.size() - 1; k++) if (!removed[k]) {
      for (auto& amb : ambiguities(G[k].lm(), f.lm())) {
        add_amb(amb, k, G.size() - 1);
      }
      for (auto& amb : ambiguities(f.lm(), G[k].lm())) {
        add_amb(amb, G.size() - 1, k);
      }
    }
  }

  std::optional<Poly<K, ord>> next() {
    if (t < G.size()) {
      Poly<K, ord> res = G[t];
      t++;
      return res;
    }
    while (!ambs.empty()) {
      auto [amb, i, j] = std::move(ambs.front());
      ambs.pop_front();

      Poly<K, ord> s = S_poly(amb, G[i], G[j]);
      reduce(s, G, removed);

      if (amb.type == Amb::Inclusion) {
        removed[i] = true;
      }

      if (!s.isZero()) {
        add_poly(s);
        t++;
        return s;
      }

    }

    return {};
  }

  std::vector<Poly<K, ord>> fullBase() {
    while (next().has_value()) {}
    std::vector<Poly<K, ord>> res;
    for (size_t i = 0; i < G.size(); i++) if (!removed[i]) {
      res.push_back(G[i]);
    }
    return res;
  }
};

enum IdealMembershipStatus {
  InIdeal,
  NotInIdeal,
  Unknown
};

template<typename K, class ord = DegLexOrd>
IdealMembershipStatus inIdeal(const std::vector<Poly<K, ord>>& G, Poly<K, ord> f, size_t max_sz = 20) {
  BuchbergerIncremental bi(G);

  for (size_t i = G.size(); i < max_sz; i++) {
    std::optional<Poly<K, ord>> p = bi.next();
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

template<typename K, class ord = DegLexOrd>
std::tuple<Poly<K, ord>, std::tuple<Monomial, Monomial, K>, std::tuple<Monomial, Monomial, K>>
S_polyReconstruct(const Amb& amb, const Poly<K, ord>& f, const Poly<K, ord>& g) {
  Monomial fa, fb, ga, gb;
  K fc, gc;
  if (amb.type == Amb::Inclusion) {
    fc = K(1) / f.lc();
    ga = amb.a;
    gb = amb.b;
    gc = K(-1) / g.lc();
  } else {
    fb = amb.b;
    fc = K(1) / f.lc();
    ga = amb.a;
    gc = K(-1) / g.lc();
  }
  return {fa * f * fb * fc + ga * g * gb * gc, {fa, fb, fc}, {ga, gb, gc}};
}

template<typename K, class ord = DegLexOrd>
struct BuchbergerIncrementalReconstruct {
  std::vector<Poly<K, ord>> G;
  std::vector<InIdealPoly<K, ord>> G_rec;
  std::vector<bool> removed;
  std::deque<std::tuple<Amb, size_t, size_t>> ambs;
  size_t t = 0;

  BuchbergerIncrementalReconstruct(std::vector<Poly<K, ord>> GG) {
    for (size_t i = 0; i < GG.size(); i++) {
      InIdealPoly<K, ord> f_rec;
      f_rec.terms.push_back({Monomial(), i, Monomial(), K(1)});
      add_poly(GG[i], f_rec);
    }
  }

  void add_amb(Amb& amb, size_t i, size_t j) {
    if (checkDeletionCriteria(G, amb, i, j)) {
      return;
    }

    ambs.push_back({std::move(amb), i, j});
  }

  void add_poly(const Poly<K, ord>& f, InIdealPoly<K, ord>& f_rec) {
    G.push_back(f);
    G_rec.push_back(std::move(f_rec));
    removed.push_back(false);
    for (size_t k = 0; k < G.size() - 1; k++) if (!removed[k]) {
      for (auto& amb : ambiguities(G[k].lm(), f.lm())) {
        add_amb(amb, k, G.size() - 1);
      }
      for (auto& amb : ambiguities(f.lm(), G[k].lm())) {
        add_amb(amb, G.size() - 1, k);
      }
    }
  }

  std::optional<Poly<K, ord>> next() {
    if (t < G.size()) {
      Poly<K, ord> res = G[t];
      t++;
      return res;
    }
    while (!ambs.empty()) {
      auto [amb, i, j] = std::move(ambs.front());
      ambs.pop_front();

      auto [s, gi_rec, gj_rec] = S_polyReconstruct(amb, G[i], G[j]);
      InIdealPoly<K, ord> s_rec = reduceReconstruct(s, G, G_rec, removed);

      if (amb.type == Amb::Inclusion) {
        removed[i] = true;
      }

      if (!s.isZero()) {
        s_rec *= K(-1);
        auto& [ai, bi, ci] = gi_rec;
        auto& [aj, bj, cj] = gj_rec;
        s_rec += ai * G_rec[i] * bi * ci;
        s_rec += aj * G_rec[j] * bj * cj;
        add_poly(s, s_rec);
        t++;
        return s;
      }

    }

    return {};
  }

  std::vector<InIdealPoly<K, ord>> fullBase() {
    while (next().has_value()) {}
    std::vector<InIdealPoly<K, ord>> res;
    for (size_t i = 0; i < G.size(); i++) if (!removed[i]) {
      res.push_back(G_rec[i]);
    }
    return res;
  }
};

template<typename K, class ord = DegLexOrd>
std::pair<IdealMembershipStatus, std::optional<InIdealPoly<K, ord>>>
inIdealReconstruct(const std::vector<Poly<K, ord>>& G, Poly<K, ord> f, size_t max_sz = 20) {
  BuchbergerIncrementalReconstruct bi(G);

  InIdealPoly<K, ord> res;
  for (size_t i = G.size(); i < max_sz; i++) {
    std::optional<Poly<K, ord>> p = bi.next();
    if (!p.has_value()) {
      return {NotInIdeal, {}};
    }
    res += reduceReconstruct(f, bi.G, bi.G_rec, bi.removed);
    if (f.isZero()) {
      return {InIdeal, res};
    }
  }

  return {Unknown, {}};
}

} // namespace nc_gb
