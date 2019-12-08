// Copyright 2019 Gavryushova Varvara
        
#ifndef MODULES_TASK_2_GAVRYUSHOVA_V_GAUS_JORDAN_GAUS_JORDAN_H_
#define MODULES_TASK_2_GAVRYUSHOVA_v_GAUS_JORDAN_GAUS_JORDAN_H_

#define MAX_NUMBER 200
#define MIN_NUMBER -100

#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include <cmath>
#include <vector>
#include <iostream>

class Matrix {
  private:
    std::vector<double> vector;
    int n;
  public:
    Matrix();
    explicit Matrix(const std::vector<double>& vec);
    explicit Matrix(const std::vector<std::vector<double>>& mat);
    Matrix(const Matrix& mat);
    ~Matrix();
    int getMatrixSize();
    double& getElem(const int row, const int col);
    std::vector<double> getLineMetod(const std::vector<double>& coefVec);
    std::vector<double> getParallelMetod(const std::vector<double>& coefVec);
};

#endif  //  MODULES_TASK_2_GAVRYUSHOVA_v_GAUS_JORDAN_GAUS_JORDAN_H_
