#include <bits/stdc++.h>
#include <givaro/qfield.h>
using namespace std;

#include "../src/F4.h"

typedef Givaro::Rational R;
typedef Poly<R> P;

int main() {

  size_t n;
  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  F4Incremental<R> F4(G);

  while (!F4.next().empty()) {}

  for (auto f : F4.G) {
    f.nice_print();
  }
}

