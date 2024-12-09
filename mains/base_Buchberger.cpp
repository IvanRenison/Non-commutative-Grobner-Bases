#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;

#include "nc_gb/Buchberger.h"

typedef mpq_class R;
typedef Poly<R> P;

int main() {

  size_t n;
  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  BuchbergerIncremental<R> bi(G);

  vector<P> base = bi.fullBase();

  cout << base.size() << endl;
  for (auto f : base) {
    f.nice_print();
  }

}

