#pragma once
#include <bits/stdc++.h>

namespace nc_gb {

template<typename K>
using Matrix = std::vector<std::vector<K>>;

// Reduce matrix to row echelon form
template<typename K>
void rref(Matrix<K>& A) {
  size_t n = A.size();
  if (n == 0) return;
  size_t m = A[0].size();
  if (m == 0) return;


  std::vector<bool> used(n, false);

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

} // namespace nc_gb
