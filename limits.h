#pragma once
#include "bdd.h"
const std::size_t N = 9;
const std::size_t M = 4;
const std::size_t N_SQRT = static_cast<size_t>(sqrt(N));
const std::size_t K = 21;
const int LOG_N = static_cast<int>(std::ceil(std::log2(N)));;

const std::size_t N_VAR = 144; // N * M * LOG_N
char var[N_VAR];
enum NeighbourRelations
{
  UP,
  DOWN
};

bdd constraint1(const bdd p[M][N][N], const int propertyNumber, const int objectNumber, const int propertyValue);
bdd constraint2(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2);
bdd constraint3(const bdd p[M][N][N], const NeighbourRelations neighbour, const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2);
bdd constraint4(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2);
void constraint5(bdd& tree, const bdd p[M][N][N]);
void constraint6(bdd& tree, const bdd p[M][N][N]);

bdd constraint1(const bdd p[M][N][N], const int propertyNumber, const int objectNumber, const int propertyValue)
{
  return p[propertyNumber][objectNumber][propertyValue];
}

bdd constraint2(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2)
{
  bdd tree = bddtrue;

  for (size_t i = 0; i < N; ++i) {
    tree &= !(p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i][propertyValue2]);
  }
  return tree;
}

bdd constraint3(const bdd p[M][N][N], const NeighbourRelations neighbour, const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2)
{
  bdd tree = bddtrue;

  switch (neighbour) {
  case UP:
    for (size_t i = N_SQRT; i < N; ++i) {
      tree &= !(p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i - N_SQRT][propertyValue2]);
    }
    break;

  case DOWN:
    for (size_t i = 0; i < N - N_SQRT; ++i) {
      tree &= (!p[propertyNumber1][i][propertyValue1] ^ p[propertyNumber2][i + N_SQRT][propertyValue2]);
    }
    break;
  }
  return tree;
}

bdd constraint4(const bdd p[M][N][N], const int propertyNumber1, const int propertyValue1, const int propertyNumber2, const int propertyValue2)
{
  return constraint3(p, NeighbourRelations::UP, propertyNumber1, propertyValue1, propertyNumber2, propertyValue2) |
    constraint3(p, NeighbourRelations::DOWN, propertyNumber1, propertyValue1, propertyNumber2, propertyValue2);
}

void constraint5(bdd& tree, const bdd p[M][N][N])
{
  for (size_t m = 0; m < M; ++m) {
    for (size_t i = 0; i < N - 1; ++i) {
      for (size_t j = i + 1; j < N; ++j) {
        for (size_t k = 0; k < N; ++k) {
          tree &= p[m][i][k] >> !p[m][j][k];
        }
      }
    }
  }
}

void constraint6(bdd& tree, const bdd p[M][N][N])
{
  for (size_t i = 0; i < N; ++i) {
    for (size_t k = 0; k < M; ++k) {
      bdd temp = bddfalse;

      for (size_t j = 0; j < N; ++j) {
        temp |= p[k][i][j];
      }

      tree &= temp;
    }
  }
}