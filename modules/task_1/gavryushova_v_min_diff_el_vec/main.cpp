//  Copyright 2019 Gavryushova Varvara

#include <gtest-mpi-listener.hpp>
#include <gtest/gtest.h>
#include <vector>
#include <iostream>
#include "../../../modules/task_1/gavryushova_v_min_diff_el_vec/min_diff_el_vec.h"

TEST(min_diff_el_vec, can_find_the_right_answer) {
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  std::vector<int> vector(5);

  if (rank == 0) {
    vector = getRandomVector(5);
  }

  double tmp = ParallelFind(vector);

  if (rank == 0) {
    ASSERT_EQ((vector), tmp);
  }
}

TEST(min_diff_el_vec, can_find_the_right_answer_with_small_vector) {
  int rank;
  std::vector<int> vector(3);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vector = getRandomVector(3);
  }

  double tmp = ParallelFind(vector);

  if (rank == 0) {
    ASSERT_EQ(ParallelFind(vector), tmp);
  }
}

TEST(min_diff_el_vec, can_find_the_right_answer_with_big_vector) {
  std::vector<int> vector(100);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vector = getRandomVector(100);
  }

  double tmp = ParallelFind(vector);

  if (rank == 0) {
    ASSERT_EQ(ParallelFind(vector), tmp);
  }
}

TEST(min_diff_el_vec, can_find_the_answer_with_very_big_vector) {
  std::vector<int> vector(1000);
  int rank;

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
    vector = getRandomVector(1000);
  }

  double tmp = ParallelFind(vector);

  if (rank == 0) {
    ASSERT_EQ(ParallelFind(vector), tmp);
  }
}

TEST(min_diff_el_vec, can_find_the_right_answer_with_even_number_elements_vector) {
  std::vector<int> vector(7);
  int rank;
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  if (rank == 0) {
     vector = getRandomVector(7);
  }

  double res = ParallelFind(vector);

  if (rank == 0) {
    ASSERT_EQ(ParallelFind(vector), res);
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
