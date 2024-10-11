#include <bits/stdc++.h>
using namespace std;

#include "../src/matrix_ModularArithmetic.h"


namespace normalMatrix {
  template<typename K>
  void rref(Matrix<K>& A) {
    size_t n = A.size();
    if (n == 0) return;
    size_t m = A[0].size();
    if (m == 0) return;


    vector<bool> used(n, false);

    // Gaussian elimination
    for (size_t c = 0; c < m; c++) {
      size_t r = 0;
      while (r < n && (used[r] || A[r][c] == K(0))) {
        r++;
      }
      if (r == n) continue;

      used[r] = true;
      K coef = A[r][c];
      for (size_t j = c; j < m; j++) {
        A[r][j] /= coef;
      }
      for (size_t i = 0; i < n; i++) {
        if (i != r && A[i][c] != K(0)) {
          K coef = A[i][c];
          for (size_t j = 0; j < m; j++) {
            A[i][j] -= coef * A[r][j];
          }
        }
      }
    }
  }
};



const size_t mod = 101;

typedef Mod<mod> M;

void test(Matrix<M> A) {
  Matrix<M> B = A;
  rref(A);
  normalMatrix::rref(B);
  sort(B.begin(), B.end());
  reverse(B.begin(), B.end());
  if (A != B) {
    cout << "A:" << endl;
    for (size_t i = 0; i < A.size(); i++) {
      for (size_t j = 0; j < A[0].size(); j++) {
        cout << A[i][j].x << " ";
      }
      cout << endl;
    }
    cout << endl;

    cout << "B:" << endl;
    for (size_t i = 0; i < B.size(); i++) {
      for (size_t j = 0; j < B[0].size(); j++) {
        cout << B[i][j].x << " ";
      }
      cout << endl;
    }
    cout << endl;

    assert(false);
  }
}

int main() {
  for (uint i = 0; i < 10000; i++) {
    size_t n = rand() % 5 + 1, m = rand() % 5 + 1;
    Matrix<M> A(n, vector<M>(m));
    for (size_t i = 0; i < n; i++) {
      for (size_t j = 0; j < m; j++) {
        A[i][j] = M(rand() % 10);
      }
    }

    test(A);
  }

  cout << "Test passed!" << endl;
}

