// Copyright 2019 Gavryushova Varvara

#include "../../../modules/task_2/gavryushova_v_gaus_jordan/gaus_jordan.h"
#include <cmath>
#include <vector>

Matrix::Matrix() {
  vector = std::vector<double>(0);
  n = 0;
}

Matrix::Matrix(const std::vector<double>& vec) {
  vector = std::vector<double>(vec);
  n = static_cast<int>(sqrt(vec.size()));
}

Matrix::Matrix(const std::vector<std::vector<double>>& mat) {
  for (size_t i = 0; i < mat.size(); ++i) {
    for (size_t j = 0; j < mat[i].size(); ++j) {
      vector.push_back(mat[i][j]);
    }
  }
  n = mat.size();
}

Matrix::Matrix(const Matrix& mat) {
  vector = mat.vector;
  n = mat.n;
}

Matrix::~Matrix() {
  vector.~vector();
}

int Matrix::getMatrixSize() {
  return n;
}

double& Matrix::getElem(const int row, const int col) {
  return vector[row * col + col];
}

std::vector<double> Matrix::getLineMetod(const std::vector<double>& coefVec) {
  std::vector<double> result(n);
  std::vector<double> additionalMat(vector);
  std::vector<double> copyCoefVec(coefVec);

  std::vector<double> x(n);
  std::vector<double> a(vector);
  std::vector<double> b(coefVec);
  for (int k = 0; k < n - 1; k++) {
    double pivot = a[k * n + k];
    for (int i = k + 1; i < n; i++) {
      double lik = a[i * n + k] / pivot;
      for (int j = k; j < n; j++)
        a[i * n + j] -= lik * a[k * n + j];
      b[i] -= lik * b[k];
    }
  }

  for (int k = n - 1; k >= 0; k--) {
    x[k] = b[k];
    for (int i = k + 1; i < n; i++)
      x[k] -= a[k * n + i] * x[i];
    x[k] /= a[k * n + k];
  }

  return x;
}

std::vector<double> Matrix::getParallelMetod(const std::vector<double>& coefVec) {
  int rank, commsize;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &commsize);
  if (coefVec.size() != static_cast<unsigned int>(n)) {
    throw(1);
  }
  if (commsize > n) {
    throw(2);
  }
  double tmp = 0;
  int colsCount = n / commsize;
  if (((n % commsize) / (rank + 1)) >= 1)
    ++colsCount;
  int aSize = colsCount * n;
  std::vector<double> a(aSize);
  size_t k = 0;
  for (int i = rank; i < n; i += commsize) {
    for (int j = 0; j < n; ++j) {
      a[k] = vector[i + j * n];
      k++;
    }
  }
  std::vector<double> recvVec;
  std::vector<double> x(coefVec);
  for (int i = 0; i < n; ++i) {
    recvVec.resize(n);
    if (i % commsize == rank) {
      recvVec = std::vector<double>(a.begin() + n * (i / commsize),
        a.begin() + n * (i / commsize) + n);
    }
    MPI_Bcast(&recvVec[0], n, MPI_DOUBLE, i % commsize, MPI_COMM_WORLD);
    tmp = recvVec[i];
    for (int j = i / commsize; j < colsCount; ++j) {
      double scaling = a[j * n + i];
      for (int k = 0; k < n; ++k) {
        if (k == i) {
          a[j * n + k] = a[j * n + k] / tmp;
        } else {
          a[j * n + k] = a[j * n + k] - round((scaling * recvVec[k] / tmp) * 100000.0) / 100000.0;
        }
      }
    }
    if (rank == 0) {
      double scaling = x[i];
      for (int k = 0; k < n; ++k) {
        if (k == i) {
          x[k] = x[k] / tmp;
        } else {
          x[k] = x[k] - round((scaling * recvVec[k] / tmp) * 100000.0) / 100000.0;
        }
      }
    }
  }
  return x;
}
