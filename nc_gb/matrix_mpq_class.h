#pragma once
#include <bits/stdc++.h>
#include <gmpxx.h>
#include <flint/fmpq.h>
#include <flint/fmpq_mat.h>
using namespace std;

#include "matrix.h"




void rref(Matrix<mpq_class>& A) {
  size_t n = A.size();
  if (n == 0) return;
  size_t m = A[0].size();
  if (m == 0) return;

  fmpq_mat_t mat_A, mat_ans;
  fmpq_mat_init(mat_A, n, m);
  fmpq_mat_init(mat_ans, n, m);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      fmpq_set_mpq(fmpq_mat_entry(mat_A, i, j), A[i][j].get_mpq_t());
    }
  }

  fmpq_mat_rref(mat_ans, mat_A);

  for (size_t i = 0; i < n; i++) {
    for (size_t j = 0; j < m; j++) {
      mpq_class q;
      fmpq_get_mpq(q.get_mpq_t(), fmpq_mat_entry(mat_ans, i, j));
      A[i][j] = q;
    }
  }
}
