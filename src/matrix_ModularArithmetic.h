#pragma once
#include <bits/stdc++.h>
using namespace std;

#include <linbox/matrix/dense-matrix.h>
#include <linbox/solutions/echelon.h>

#include "../extras/ModularArithmetic.h"
#include "matrix.h"

// Specialization of rref for ModularArithmetic
template <size_t mod>
void rref(Matrix<Mod<mod>>& A) {
  size_t n = A.size(), m = A[0].size();

  LinBox::DenseMatrix<Field> B(F<mod>);
  B.resize(n, m);
  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      B.refEntry(i, j) = A[i][j].x;
    }
  }

  reducedRowEchelonize(B);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      A[i][j].x = B.getEntry(i, j);
    }
  }
}
