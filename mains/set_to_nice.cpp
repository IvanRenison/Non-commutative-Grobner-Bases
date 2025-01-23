#include <bits/stdc++.h>
#include <gmpxx.h>

#include "ncgb/nc_polynomial.hpp"

using namespace std;
using namespace ncgb;

typedef mpq_class R;
typedef Poly<R> P;

int main() {

  size_t n;
  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  for (auto f : G) {
    f.nice_print();
  }

}

