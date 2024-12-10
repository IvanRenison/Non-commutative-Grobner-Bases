#include <bits/stdc++.h>
#include <gmpxx.h>
#include <omp.h>

#include "ncgb/F4.h"

using namespace std;
using namespace ncgb;

typedef mpq_class R;
typedef Poly<R> P;

int main() {
  omp_set_num_threads(8);

  size_t n;
  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  F4Incremental<R> F4(G);

  vector<P> base = F4.fullBase();

  cout << base.size() << endl;
  for (auto f : base) {
    f.nice_print();
  }
}

