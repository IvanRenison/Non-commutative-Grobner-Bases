#include <bits/stdc++.h>
using namespace std;

#include "../src/Buchberger.h"
#include "../extras/Rational.h"


typedef Poly<Ratio> P;

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

