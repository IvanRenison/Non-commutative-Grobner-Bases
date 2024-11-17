#include <bits/stdc++.h>
#include <gmpxx.h>
using namespace std;

#include "../src/cmpBases.h"

typedef mpq_class R;
typedef Poly<R> P;

/*
Using nice input in this file because GB output is in that format
*/

int main(int argc, char *argv[]) {
  if (argc != 3) {
    cout << "Usage: " << argv[0] << " file0 file1" << endl;
    return 1;
  }

  string file0 = argv[1], file1 = argv[2];

  ifstream in0(file0), in1(file1);

  size_t n0, n1;
  in0 >> n0;
  in0.ignore();
  in1 >> n1;
  in1.ignore();
  vector<P> G0(n0), G1(n1);
  for (size_t i = 0; i < n0; ++i) {
    G0[i] = P::nice_read(in0);
  }
  cout << endl;
  for (size_t i = 0; i < n1; ++i) {
    G1[i] = P::nice_read(in1);
  }

  bool equiv = cmpBases(G0, G1);
  if (!equiv) {
    cout << "Not equivalent" << endl;
  } else {
    cout << "Equivalent" << endl;
  }
}
