#include <bits/stdc++.h>
#include <gmpxx.h>

#include "ncgb/Buchberger.h"

using namespace std;
using namespace ncgb;


typedef Poly<mpq_class> P;

int main() {

  size_t n;

  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  P f;
  P g0;
  cin >> f;
  size_t k;
  cin >> k;

  k += G.size();

  IdealMembershipStatus ims = inIdeal(G, f, k);

  if (ims == InIdeal) {
    cout << "InIdeal\n";
  } else {
    cout << "NotInIdeal or Unknown\n";
  }

}

