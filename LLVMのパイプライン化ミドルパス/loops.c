#include <stdio.h>
#include <stdlib.h>

#ifdef __INTELLISENSE__
#define __attribute__(X)
#endif

__attribute__((noinline)) void func_parallel(size_t N, float *restrict A,
                                             float *restrict B,
                                             float *restrict C,
                                             float *restrict D) {
  for (size_t i = 0; i < N; ++i) {
    A[i] = A[i] + B[i] + C[i] + D[i];
  }
}

__attribute__((noinline)) void func_orig(size_t N, float *restrict A,
                                         float *restrict B, float *restrict C) {
  for (size_t i = 0; i < N; ++i) {
    A[i + 1] = (C[i] + B[i]) + A[i];
  }
}

//ここを２段パイプラインにする
__attribute__((noinline)) void
func_pragma(size_t N, float *restrict A, float *restrict B, float *restrict C) {
#pragma clang loop pipelining(enable)
  for (size_t i = 0; i < N; ++i) {
    A[i + 1] = (C[i] + B[i]) + A[i];
  }
}

__attribute__((noinline)) void func_pipe_by_hand(size_t N, float *restrict A,
                                                 float *restrict B,
                                                 float *restrict C) {
  float X;

  X = C[0] + B[0];
  for (size_t i = 1; i < N; ++i) {
    A[i] = X + A[i - 1];
    X = C[i] + B[i];
  }
  A[N] = X + A[N - 1];
}

int main(int argc, char *argv[]) {
  printf("loops.c start\n");
  if (argc < 3) {
    return 1;
  }
  size_t lhs = atoi(argv[1]);
  size_t rhs = atoi(argv[2]);
  size_t size = rhs - lhs;
  float *A1 = calloc(sizeof(float), size);
  float *A2 = calloc(sizeof(float), size);
  float *A3 = calloc(sizeof(float), size);
  float *B = calloc(sizeof(float), size);
  float *C = calloc(sizeof(float), size);

  for (size_t i = 0; i < size; ++i) {
    B[i] = (float)lhs + i;
    C[i] = (float)lhs + i;
  }

  func_orig(size - 1, A1, B, C);
  printf("func_orig _complete result is A1\n");
  func_pragma(size - 1, A2, B, C);
  printf("func_pragma complete result is A2\n");
  func_pipe_by_hand(size - 1, A3, B, C);
  printf("func_pipe_by_hand result is A3\n");

  size_t error = 0;
  for (size_t i = 0; i < size; ++i) {
    if (A1[i] != A2[i]) {
      error = 1;
      printf("A1-A2 mismatch: %d\n", i);
    }
    if (A2[i] != A3[i]) {
      error = 1;
      printf("A2-A3 mismatch: %d\n", i);
    }
    if (A3[i] != A1[i]) {
      error = 1;
      printf("A3-A1 mismatch: %d\n", i);
    }
  }
  printf("loops.c check complete%s\n", error ? " :error found" : " :result OK");

  return 0;
}
