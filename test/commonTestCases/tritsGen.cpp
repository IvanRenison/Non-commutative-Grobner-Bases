#include <bits/stdc++.h>

#include "ncgb/nc_polynomial.hpp"

using namespace std;
using namespace ncgb;

vector<vector<string>> trits = {
  {"aaaaa", "bb", "ccc", "ababab", "aacaac", "bcbc"},
  {"aaa", "bbb", "ccccc", "abab", "acac", "bccbcc"},
  {"aaa", "bbb", "ccc", "abab", "acac", "bcbc"},
  {"aaa", "bbb", "cccc", "abab", "acac", "bcbc"},
  {"aaa", "bbb", "ccccc", "abab", "acac", "bcbc"}
};


int main() {
  int n;
  cin >> n;

  assert(n - 1 < trits.size());

  vector<Poly<int, __uint64_t>> polys;
  for (string s : trits[n - 1]) {
    stringstream ss("1 " + s + " - 1");
    Poly<int, __uint64_t> p;
    p = Poly<int, __uint64_t>::nice_read(ss);
    polys.push_back(p);
  }

  cout << polys.size() << endl;
  for (auto p : polys) {
    cout << p;
  }
}
