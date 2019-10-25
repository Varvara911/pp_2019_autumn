// Copyright 2019 Gavryushova Varvara

#include <iostream>
#include <cmath>
#include <vector>
#include <random>
#include "../../../modules/task_1/gavryushova_v_min_diff_el_vec/min_diff_el_vec.h"
#include "./mpi.h"

std::vector<int> getRandomVector(int n) { //  создание случайного вектора
  std::mt19937 random;

  std::vector<int> vector;
  for (int i = 0; i < n; i++)
    vector.push_back(random() % 10);
  return vector;
}

int LineFind(std::vector<int> vec) {  // линейный поиск
  if (vec.size() - 1 < 2) {  // проверка вектора на подходящий размер(слишком маленький)
    throw("the vector is small");
  }

  int min = 0; // минимальная разница
  int i;
  for (unsigned int k; k < vec.size(); k++) {
    if (abs(vec[k] - vec[k + 1]) < min) { // если разница между элементами меньше min
      min = abs(vec[k] - vec[k + 1]);  // то записываем новый min
    }
  }

  return min;
}

int ParallelFind(std::vector<int> vec) {  //параллельный поиск
  if (vec.size() - 1 < 2) {  // проверка требований размера(слишком маленький)
    throw("small size of vector");
  }

  int rank, size;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  int count = 1, rec = 0;

  if ((unsigned int)size < vec.size() - 1) {
    count = (vec.size() - 1) / size;
    rec = (vec.size() - 1) % size;
  }

  if (rank == 0) {
    for (int proc = 1; proc < size; proc++) {
      MPI_Send(&vec[0] + proc * count + rec, count + 1, MPI_INT, proc, 0, MPI_COMM_WORLD);
    }
  }

  std::vector<int> mpi_vector(count + 1);

  if (rank == 0) {
    mpi_vector = std::vector<int>(vec.begin(), vec.begin() + rec + count + 1);
  }
  else {
    MPI_Status status;
    MPI_Recv(&mpi_vector[0], count + 1, MPI_INT, 0, 0, MPI_COMM_WORLD, &status);
  }

  int minimum = 0, mpi_min = 0;

  if (rank < vec.size() - 1) {
    for (unsigned int i = 0; i < mpi_vector.size() - 1; i++) {  //проход по локальному вектору
      if (abs(mpi_vector[i] - mpi_vector[i + 1]) < mpi_min) {
        mpi_min = abs(mpi_vector[i] - mpi_vector[i + 1]);
      }
    }
  }
  MPI_Reduce(&mpi_min, &minimum, 1, MPI_INT, MPI_MAX, 0, MPI_COMM_WORLD);
  return minimum;
}
