#include <bits/stdc++.h>
using namespace std;

#include "nc_gb/Zfunc.h"

template <typename Iterator>
vector<size_t> slow_Z(Iterator start, Iterator end) {
  size_t n = distance(start, end);
  vector<size_t> z(n);
  for (size_t i = 1; i < n; i++) {
    while (i + z[i] < n && *(start + z[i]) == *(start + i + z[i])) {
      z[i]++;
    }
  }
  return z;
}

void test() {
  size_t n = rand() % 100 + 1;
  vector<size_t> v(n);
  generate(v.begin(), v.end(), []() { return rand() % 100; });

  vector<size_t> z = Z(v);
  vector<size_t> slow_z = slow_Z(v.begin(), v.end());
  assert(z == slow_z);
}

int main() {
  for (uint i = 0; i < 1000; i++) {
    test();
  }

  cout << "Test passed!" << endl;
}
