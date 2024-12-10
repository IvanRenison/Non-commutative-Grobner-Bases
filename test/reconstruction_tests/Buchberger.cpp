#include <bits/stdc++.h>
#include <gmpxx.h>

#include "ncgb/Buchberger.h"
#include "ncgb/cmpBases.h"

using namespace std;
using namespace ncgb;

typedef mpq_class R;
typedef Poly<R> P;

/*
Assuming the generator set given by input has a finite base, tests that
the reconstruction is valid
*/
int main() {

  size_t n;
  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  // Messure time
  auto start = chrono::high_resolution_clock::now();
  BuchbergerIncremental<R> bi(G);
  vector<P> base = bi.fullBase();
  auto end = chrono::high_resolution_clock::now();

  cerr << "Normal time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;

  start = chrono::high_resolution_clock::now();
  BuchbergerIncrementalReconstruct<R> bir(G);
  vector<InIdealPoly<R>> in_ideal_base = bir.fullBase();
  end = chrono::high_resolution_clock::now();
  cerr << "Reconstruction time: " << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << endl;
  vector<P> baser;
  for (auto& p : in_ideal_base) {
    baser.push_back(p.construct(G));
  }

  if (base != baser) {
    cerr << "Base reconstruction failed" << endl;
    cerr << "Normal base:" << endl;
    for (auto& p : base) {
      p.nice_print(cerr);
    }
    cerr << "Reconstructed base:" << endl;
    for (auto& p : baser) {
      p.nice_print(cerr);
    }
    return 1;
  }
}
