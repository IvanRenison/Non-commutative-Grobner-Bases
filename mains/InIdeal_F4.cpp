#include <bits/stdc++.h>
#include <gmpxx.h>
#include <omp.h>

#include "ncgb/matrix_mpq_class.h"
#include "ncgb/F4.h"

using namespace std;
using namespace ncgb;

typedef Poly<mpq_class> P;

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

