/*
This file is the same that mains/base_F4.cpp but reding the amount of threads by command line
*/
#include <bits/stdc++.h>
#include <gmpxx.h>
#include <omp.h>

#include "ncgb/F4.hpp"

using namespace std;
using namespace ncgb;

typedef mpq_class R;
typedef Poly<R> P;

int main(int argc, char** argv) {
  assert(argc == 2);
  int threads = atoi(argv[1]);
  assert(threads > 0);
  omp_set_num_threads(threads);

  size_t n;
  cin >> n;
  vector<P> G(n);
  for (size_t i = 0; i < n; ++i) {
    cin >> G[i];
  }

  F4Incremental<R> F4(G);

  vector<P> base = F4.fullBase();

  cout << base.size() << endl;
  for (auto f : base) {
    f.nice_print();
  }
}

