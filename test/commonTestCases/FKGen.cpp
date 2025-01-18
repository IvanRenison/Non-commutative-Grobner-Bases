#include <bits/stdc++.h>
using namespace std;

int main() {
  size_t n;
  cin >> n;

  vector<vector<size_t>> vars(n, vector<size_t>(n));
  for (size_t x = 0, i = 0; i < n; i++) {
    for (size_t j = 0; j < i; j++) {
      vars[i][j] = vars[j][i] = x;
      x++;
    }
  }

  size_t count = 0;
  stringstream ss;
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < i; j++) {
      ss << "1\n1 2 " << vars[i][j] << ' ' << vars[i][j] << '\n';
      count++;
    }
  }
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < i; j++) {
      for (size_t k = 0; k < j; k++) {
        ss << "3\n";
        ss << "1 2 " << vars[i][j] << ' ' << vars[i][k] << '\n';
        ss << "1 2 " << vars[i][k] << ' ' << vars[j][k] << '\n';
        ss << "1 2 " << vars[j][k] << ' ' << vars[i][j] << '\n';
        count++;
      }
    }
  }
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < i; j++) {
      for (size_t k = 0; k < j; k++) {
        ss << "3\n";
        ss << "1 2 " << vars[i][k] << ' ' << vars[i][j] << '\n';
        ss << "1 2 " << vars[j][k] << ' ' << vars[i][k] << '\n';
        ss << "1 2 " << vars[i][j] << ' ' << vars[j][k] << '\n';
        count++;
      }
    }
  }
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < i; j++) {
      for (size_t k = 0; k < i; k++) if (k != j) {
        for (size_t l = 0; l < k; l++) if (l != j) {
          ss << "2\n";
          ss << "1 2 " << vars[i][j] << ' ' << vars[k][l] << '\n';
          ss << "1 2 " << vars[k][l] << ' ' << vars[i][j] << '\n';
          count++;
        }
      }
    }
  }
  cout << count << '\n' << ss.str();

}
