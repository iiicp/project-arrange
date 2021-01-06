/***********************************
 * File:     openblastest.h
 *
 * Author:   caipeng
 *
 * Email:    iiicp@outlook.com
 *
 * Date:     2021/1/5
 ***********************************/

#ifndef PERFORMANCE_OPTIMAZING_OPENBLASTEST_H
#define PERFORMANCE_OPTIMAZING_OPENBLASTEST_H

#include <Accelerate/Accelerate.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

/**
 * Moving a local object in a return statement prevents copy elision
 *
 * https://www.zhihu.com/question/57048704/answer/151446405
 *
 * C++ 类型判断
 * std::is_same<T1, T2>
 */

namespace OpenblasTest {
template <typename T> struct Matrix {
  int row;
  int col;
  T *data;
  Matrix(int row, int col) {
    assert(row > 0 && col > 0);
    this->row = row;
    this->col = col;
    data = new T[row * col];

    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < col; ++j) {
        data[i * col + j] = 0;
      }
    }
  }

  /// move 构造函数签名是非const，因为要把内存存储区域设置为NULL
  Matrix(Matrix &&mat) : row(mat.row), col(mat.col), data(mat.data) {
    printf("move matrix...\n");
    mat.data = NULL;
  }

  void randomize() {
    srand((unsigned)time(nullptr));
    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < col; ++j) {
        if (std::is_same<T, int>::value)
          data[i * col + j] = (rand() % 10);
        else {
          data[i * col + j] = (rand() % 10) * 1.0f;
        }
      }
    }
  }

  void dumpMat() {
    for (int i = 0; i < row; ++i) {
      for (int j = 0; j < col; ++j) {
        std::cout << data[i * col + j] << " ";
      }
      printf("\n");
    }
    printf("\n");
  }

  ~Matrix() {
    if (data) {
      delete[] data;
    }
  }
};

template <typename T> struct Vec {
  int row;
  T *data;

  Vec(const Vec &v) {
    std::cout << "copy construct..." << std::endl;
    row = v.row;
    data = new T[row];
    for (int i = 0; i < v.row; ++i)
      data[i] = v.data[i];
  }

  Vec(Vec &&v) : data(v.data), row(v.row) {
    std::cout << "move construct..." << std::endl;
    v.data = NULL;
  }

  Vec &operator=(const Vec &v) {
    std::cout << "operator=" << std::endl;
    row = v.row;
    for (int i = 0; i < row; ++i)
      data[i] = v.data[i];
    return *this;
  }

  Vec(int row) {
    assert(row > 0);
    this->row = row;
    data = new T[row];
    for (int i = 0; i < row; ++i)
      data[i] = 0;
  }

  void randomize() {
    srand((unsigned)time(nullptr));
    for (int i = 0; i < row; ++i)
      data[i] = (rand() % 10) * 1.0f;
  }

  void dumpVec() {
    printf("\n");
    for (int i = 0; i < row; ++i)
      std::cout << data[i] << " ";
    printf("\n");
  }

  T &operator[](int i) { return data[i]; }

  ~Vec() {
    if (data) {
      delete[] data;
    }
  }
};
class MyTest {
public:
  /// y = alpha * x + y;
  void VecAddVec(Vec<float> &x, Vec<float> &y, float alpha) {
    assert(x.row == y.row);
    int n = x.row;
    for (int i = 0; i < n; ++i) {
      y[i] += x[i] * alpha;
    }
  }

  /// y = alpha * x + y; (小规模向量会负优化，至少要大于512)
  void VecAddVec_blas(Vec<float> &x, Vec<float> &y, float alpha) {
    assert(x.row == y.row);
    int n = x.row;
    cblas_saxpy(n, alpha, x.data, 1, y.data, 1);
  }

  /// C = alpha * A * b + beta * C
  Vec<float> MatMulVec(Matrix<float> &A, Vec<float> &b) {
    assert(A.col == b.row);

    Vec<float> c(A.row);

    int M = A.row, N = A.col;
    for (int i = 0; i < M; ++i) {
      for (int j = 0; j < N; ++j) {
        c[i] += A.data[i * N + j] * b[j];
      }
    }

    return c;
  }

  /// C = alpha * A * b + beta * C
  Vec<float> MatMulVec_blas(Matrix<float> &A, Vec<float> &b) {
    assert(A.col == b.row);

    Vec<float> c(A.row);

    int M = A.row, N = A.col;
    float alpha = 1.0f, beta = 0.0f;
    int lda = A.col;
    cblas_sgemv(CblasRowMajor, CblasNoTrans, M, N, alpha, A.data, lda, b.data,
                1, beta, c.data, 1);

    return c;
  }

  /// C = alpha * A * B + beta * C
  Matrix<float> MatMulMat(Matrix<float> &A, Matrix<float> &B) {
    assert(A.col == B.row);

    Matrix<float> C(A.row, B.col);
    int M = A.row, N = B.col, K = B.row;
    for (int i = 0; i < M; ++i) {
      for (int j = 0; j < N; ++j) {
        for (int k = 0; k < K; ++k) {
          C.data[i * N + j] += A.data[i * A.col + k] * B.data[k * N + j];
        }
      }
    }
    return C;
  }

  /// C = alpha * A * B + beta * C
  Matrix<float> MatMulMat_blas(Matrix<float> &A, Matrix<float> &B) {
    assert(A.col == B.row);

    Matrix<float> C(A.row, B.col);

    int M = A.row, N = B.col, K = B.row;
    float alpha = 1.0f, beta = 0.0f;
    int lda = A.col, ldb = B.col, ldc = B.col;
    cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, M, N, K, alpha,
                A.data, lda, B.data, ldb, beta, C.data, ldc);

    return C;
  }
};
} // namespace OpenblasTest

#endif // PERFORMANCE_OPTIMAZING_OPENBLASTEST_H
