#include <bits/stdc++.h>
#include <givaro/qfield.h>
using namespace std;

#include "../src/Buchberger.h"

typedef Givaro::Rational R;
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

  for (auto f : bi.G) {
    f.nice_print();
  }

}

