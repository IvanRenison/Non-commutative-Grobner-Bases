#include <bits/stdc++.h>
#include <omp.h>

#include "nc_gb/F4.h"
#include "extras/ModularArithmetic.h"

using namespace std;
using namespace nc_gb;

const size_t mod = 1e9 + 7;
typedef Mod<mod> Zp;

typedef Poly<Zp> P;

int main() {
  omp_set_num_threads(8);

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

  IdealMembershipStatus ims = inIdeal_F4(G, f, k);

  if (ims == InIdeal) {
    cout << "InIdeal\n";
  } else {
    cout << "NotInIdeal or Unknown\n";
  }

}

