// Copyright 2019 Gavryushova Varvara

#include <time.h>
#include <gtest/gtest.h>
#include <gtest-mpi-listener.hpp>
#include <random>
#include <vector>
#include "./gaus_jordan.h"

TEST(gaus_jordan, calculate_matrix_correct_using_line_method) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> vect{ {1, 1, 1}, {2, 3, 2}, {3, 5, 7} };
  Matrix mat(vect);
  std::vector<double> result(mat.getMatrixSize());
  if (rank == 0) {
    result = mat.getLineMetod({ 1, 1, 1 });
    EXPECT_NEAR(result[0], 2.0, 0.00001);
    EXPECT_NEAR(result[1], -1.0, 0.00001);
    EXPECT_NEAR(result[2], 0.0, 0.00001);
  }
}

TEST(gaus_jordan, calculate_matrix_correct_using_parallel_method) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> vect{ {1, 1, 1}, {2, 3, 2}, {3, 5, 7} };
  Matrix mat(vect);
  std::vector<double> res(mat.getMatrixSize());
  res = mat.getParallelMetod({ 1, 1, 1 });
  if (rank == 0) {
    EXPECT_NEAR(res[0], 2.0, 0.00001);
    EXPECT_NEAR(res[1], -1.0, 0.00001);
    EXPECT_NEAR(res[2], 0.0, 0.00001);
  }
}

TEST(gaus_jordan, throw_if_matrix_size_less_then_process_count) {
  Matrix mat;
  ASSERT_ANY_THROW(mat.getParallelMetod({ 1, 1, 1 }));
}

TEST(gaus_jordan, throw_if_matrix_size_not_equal_result_vector_size) {
  std::vector<std::vector<double>> vect{ {1, 1, 1}, {2, 3, 2}, {3, 5, 7} };
  Matrix mat(vect);
  std::vector<double> res(mat.getMatrixSize());
  ASSERT_ANY_THROW(mat.getParallelMetod({ 1, 1, 1, 1 }));
}

TEST(gauss_method, can_calculate_matrix_with_size_in_4_elements) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<std::vector<double>> vect{ { 3, 3, 6, 3 }, { 3, 1, 5, 1 }, { 2, 1, 4, 2 },
    { 1, 3, 3, 2 } };
  Matrix mat(vect);
  std::vector<double> res(mat.getMatrixSize());
  res = mat.getParallelMetod({ 6, 2, 1, 6});
  if (rank == 0) {
    EXPECT_NEAR(res[0], 3.000, 0.001);
    EXPECT_NEAR(res[1], 3.000, 0.001);
    EXPECT_NEAR(res[2], -2.000, 0.001);
    EXPECT_NEAR(res[3], 0.000, 0.001);
  }
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  MPI_Init(&argc, &argv);

  ::testing::AddGlobalTestEnvironment(new GTestMPIListener::MPIEnvironment);
  ::testing::TestEventListeners& listeners =
  ::testing::UnitTest::GetInstance()->listeners();

  listeners.Release(listeners.default_result_printer());
  listeners.Release(listeners.default_xml_generator());

  listeners.Append(new GTestMPIListener::MPIMinimalistPrinter);
  return RUN_ALL_TESTS();
}
