#include <bits/stdc++.h>

#include "ncgb/nc_polynomial.hpp"

using namespace std;
using namespace ncgb;

vector<vector<string>> tris = {
  {"aa", "bbb", "abababbabbabababbabb"}, // tri1
  {"aa", "bbb", "abababbabababbabababb"}, // tri2
  {"aaa", "bbb", "ababbababb"}, // tri3
  {"aaa", "bbb", "abaabbabaabb"}, // tri4
  {"aa", "bbbbb", "ababbababb"}, // tri5
  {"aa", "bbbbb", "abababbbbabababbbb"}, // tri6
  {"aa", "bbbbb", "ababbabbbbababbabbbb"}, // tri7
  {"aa", "bbbb", "abababbbabababbb"}, // tri8
  {"aa", "bbb", "ababbababb"}, // tri9
  {"aa", "bbb", "abababbabababb"}, // tri10
  {"aa", "bbb", "ababababbababababb"}, // tri11
  {"aa", "bbb", "abababbababbabababbababb"}, // tri12
  {"aa", "bbb", "bababababbabbbababababbabb"}, // tri13
  {"aa", "bbb", "ababababbabbababbabbababababbabbababbabb"} // tri14
};


int main() {
  int n;
  cin >> n;

  assert(n - 1 < tris.size());

  vector<Poly<int, __uint64_t>> polys;
  for (string s : tris[n - 1]) {
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
