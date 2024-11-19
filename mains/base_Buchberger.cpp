#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;

#include "Buchberger.h"

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

  while (bi.next().has_value()) {}

  cout << bi.G.size() << endl;
  for (auto f : bi.G) {
    f.nice_print();
  }

}

